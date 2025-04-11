<script setup>

import { onMounted, ref } from 'vue';
import HeaderComponent from '../components/Header.vue'
import { showSuccessAlert, showErrorAlert } from '../utils/alerts.js'
import Ram from './../store/ram.js'
import { useRouter } from 'vue-router'


const router = useRouter()

const configured = ref(Ram.configured)

onMounted(async() => {
    await Ram.fetchMatrixRepresentation()
    configured.value = Ram.configured

});

const onPlayClicked = async() => {
    // Handle play button click
    console.log('Play button clicked');
    if(configured.value === 'True') {

        const response = await fetch('http://localhost:3000/game/play.json', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({}), // Adjust the coordinates as needed
        });

        if (!response.ok) {
            const errorData = await response.json();  // Parse the error JSON
            showErrorAlert('Error', errorData.message || 'Error when playing the game');
            return;
        }

        // const data = await response.json();

        router.push('/game')
    } else {
        showErrorAlert('Error', 'Please configure the bombs first.')
    }
};

</script>

<template>
    <div class="home">
        <HeaderComponent/>
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
                    <a @click.prevent="onPlayClicked">
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
                    </a>
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