<script setup>

import { onMounted, ref } from 'vue';
import HeaderComponent from '../components/Header.vue'
import { showSuccessAlert, showErrorAlert } from '../utils/alerts.js'
import Ram from './../store/ram.js'
import { useRouter } from 'vue-router'


const router = useRouter()

const configured = ref(Ram.configured)
let game_mode = ref('')
let status = ref('')

onMounted(async() => {
    await Ram.fetchMatrixRepresentation()
    configured.value = Ram.configured

});

const onPlayClicked = async() => {
    // Handle play button click
    console.log('Play button clicked');

    if(status.value == 'playing') {
        router.push('/game')
        return;
    }

    if(configured.value === 'True') {

        const response = await fetch('http://localhost:3000/game/play.json', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ game_mode: game_mode.value}), // Adjust the coordinates as needed
        });

        if (!response.ok) {
            const errorData = await response.json();  // Parse the error JSON
            showErrorAlert('Error', errorData.message || 'Error when playing the game');
            return;
        }

        Ram.game_mode = game_mode.value
        Ram.status = 'playing'

        router.push('/game')
    } else {
        showErrorAlert('Error', 'Please configure the bombs first.')
    }
};

const showTop5 = async() => {
    const response = await fetch('http://localhost:3000/game/top5.json', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({}), // Adjust the coordinates as needed
    });

    
    if (!response.ok) {
        const errorData = await response.json();  // Parse the error JSON
        showErrorAlert('Error', errorData.message || 'Error when showing Top5');
        return;
    }

    showSuccessAlert('Top5', 'Top5 shown successfully');
}

const resetGame = async() => {
    const response = await fetch('http://localhost:3000/game/reset.json', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({}), // Adjust the coordinates as needed
    });

    if (!response.ok) {
        const errorData = await response.json();  // Parse the error JSON
        showErrorAlert('Error', errorData.message || 'Error when showing top');
        return;
    }

    showSuccessAlert('Reset', 'Reset successfully');

    Ram.configured = false
    Ram.bombs = []
    Ram.game_mode = ''
    Ram.status = 'not_started'
    configured.value = false
    status.value = 'not_started'
}

onMounted(() => {
    game_mode.value = Ram.game_mode
    status.value = Ram.status
});

</script>

<template>
    <div class="home">
        <HeaderComponent headerMode="Home"/>
        <form v-if="status != 'playing'" class="has-text-centered" @submit.prevent="showTop5">
            <button type="submit" class="button is-black">
                Show Top 5
            </button>
        </form>
        <form class="has-text-centered mt-1" @submit.prevent="resetGame">
            <button type="submit" class="button is-info">
                Reset
            </button>
        </form>
        <div class="fixed-grid container">
            <div class="grid is-flex has-align-items-center is-justify-content-space-around">
                <div class="cell">
                    <RouterLink to="/settings">
                        <div class="card-home-content card-home-content-settings settings has-text-light has-text-weight-bold">
                            <div class="mb-4">
                                <span class="icon-text">
                                    <span class="icon">
                                        <a href="#" class="fas fa-gear config-icon"></a>
                                    </span>
                                </span>
                            </div>
                            <p>Settings</p>
                        </div>
                    </RouterLink>
                </div>
                <div class="cell">
                    <form class="form has-text-right form-play" @submit.prevent="onPlayClicked">
                        <button>
                            <div class="card-home-content card-home-content-play has-text-dark has-text-weight-bold" disabled>
                                <div class="mb-4">
                                    <span class="icon-text">
                                        <span class="icon">
                                            <a class="fas fa-play play-icon entry-settings">
                                            </a>
                                        </span>
                                    </span>
                                </div>
                                <p>Play</p>
                            </div>
                        </button>
                        <div className="select">
                            <select v-model="game_mode" required :disabled="status != 'not_started'">
                                <option disabled>Game mode</option>
                                <option>Web</option>
                                <option>Keypad</option>
                                <option>Bluetooth</option>
                            </select>
                        </div>
                    </form>
                </div>
            </div>
            <article v-if="configured != 'True'" class="message is-warning">
                <div class="message-body">
                    <p>There are NO BOMBS configured!</p>
                </div>
            </article>
        </div>
    </div>
</template>