<script setup>
import Ram from '../store/ram.js'
import { showSuccessAlert, showErrorAlert } from '../utils/alerts.js'
import { defineProps, ref, onMounted } from 'vue';
import { useRouter } from 'vue-router'
const router = useRouter()

const props = defineProps({
    mode: {
        type: String,
        required: true,
    }
})

const coordinates = ref([]);
const fileInput = ref(null); // Referencia para el input de archivo oculto
let game_mode = ref('')

// Función para manejar el clic en el botón Import
const handleImportClick = () => {
    fileInput.value.click(); // Dispara el click en el input de archivo
};

// Función para manejar la selección de archivos
const handleFileUpload = async (event) => {
    const file = fileInput.value.files[0];
    if (!file) {
        alert('Por favor selecciona un archivo.');
        return;
    }

    // Leer el archivo como texto
    const reader = new FileReader();
    reader.onload = async () => {
        const fileContent = reader.result; // Contenido del archivo como texto

        try {
        const response = await fetch('http://localhost:3000/bombs/import.json', {
            method: 'POST',
            headers: {
            'Content-Type': 'application/json',
            },
            body: JSON.stringify({ content: fileContent }), // Enviar el contenido como JSON
        });

        if (!response.ok) {
            showErrorAlert('Error', 'Error upploading the file');
            return;
        }

        const response_data = await response.json();
        showSuccessAlert('Success', `File bombs uploaded successfully, ${response_data.content.length} bombs added`);

        response_data.content.forEach((coord) => {
            const x = coord.x - 1;
            const y = coord.y - 1;

            console.log(`x: ${x}, y: ${y}`);

            document.querySelector(`#cell-board-${x}-${y}`).classList.add('is-danger');
            Ram.matrix_representation[x][y] = 1; // Update the matrix representation
        })

        } catch (error) {
            console.error(error);
            alert('Hubo un error al enviar el contenido');
        } finally {
            fileInput.value.value = null;
        }
    };

    reader.onerror = (error) => {
        console.error(error);
        alert('Hubo un error al leer el archivo');
    };

    reader.readAsText(file); // Leer el archivo como texto
    fileInput.value.value = null;
};



// Resto de tus funciones existentes (verify, createBomb, verifyBomb)...
const verify = (e, x, y, value) => {
    e.preventDefault();

    if(props.mode.toLocaleLowerCase() === 'game') {
        console.log('Game mode');
        verifyBomb(e, x, y)
    } else if (props.mode.toLocaleLowerCase() === 'settings') {
        console.log('Settings mode');
        createBomb(e, x, y, value)
    } else {
        console.log('Unknown mode');
    }
};

const createBomb = async (e, x, y, value) => {
    const response = await fetch('http://localhost:3000/bombs.json', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ x: x + 1, y: y + 1 }), // Adjust the coordinates as needed
    });

    if (!response.ok) {
        const errorData = await response.json();  // Parse the error JSON
        showErrorAlert('Error', errorData.message || 'Error when adding the bomb');
        return;
    }

    const data = await response.json();

    document.querySelector(`#cell-board-${x}-${y}`).classList.add('is-danger');

    showSuccessAlert('Bomb added', 'The bomb has been added successfully');

    Ram.matrix_representation[x][y] = 1;
};

const verifyBomb = async(e, x, y) => {
    const response = await fetch('http://localhost:3000/bombs/verify.json', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ x: x + 1, y: y + 1 }), // Adjust the coordinates as needed
    })

    if (!response.ok) {
        const errorData = await response.json();  // Parse the error JSON
        showErrorAlert('Error', errorData.message || 'Error al agregar la bomba');
        return;
    }

    const data = await response.json();

    if(data === "BOOM"){
        showErrorAlert('BOOM', 'Has perdido');
        document.querySelector(`#cell-game-${x}-${y}`).classList.add('is-danger');
        Ram.game_mode = '';
        Ram.status = 'not_started'
        router.push('/')
    }else if(data === "SAFE"){
        document.querySelector(`#cell-game-${x}-${y}`).classList.add('is-primary');
        
        setTimeout(() => {
            document.querySelector(`#cell-game-${x}-${y}`).classList.remove('is-primary');
        }, 1000);

    } else if(data === "WIN"){
        showSuccessAlert('WIN', 'Has ganado');
        document.querySelector(`#cell-game-${x}-${y}`).classList.add('is-success');
        router.push('/')
        Ram.status = 'not_started'
    }
};

onMounted(async () => {
    await Ram.fetchMatrixRepresentation()
    coordinates.value = Ram.matrix_representation
    console.log(`Mode: ${props.mode}`);
    game_mode.value = Ram.game_mode
});
</script>

<template>
    <section class="bombs-board-section has-text-centered">
        <form class="form has-text-right" v-if="mode == 'settings'" @submit.prevent>
            <!-- Input de archivo oculto -->
            <input 
                type="file" 
                ref="fileInput" 
                style="display: none" 
                accept=".org" 
                @change="handleFileUpload"
            />
            <button 
                class="button is-info" 
                @click="handleImportClick"
                type="button">
                <span class="icon-text">
                    <span class="icon">
                        <i class="fa-solid fa-upload"></i>
                    </span>
                    <span>Import</span>
                </span>
            </button>
        </form>
        <br/>
        <br/>
        <form class="form">
            <div class="columns" v-for="(valuex, indexx) in coordinates" :key="indexx">
                <div class="column" v-for="(valuey, indexy) in valuex" :key="indexy">
                    <div class="bombs-board-item">
                        <button
                            v-if="mode == 'settings'" 
                            @click="(e) => verify(e, indexx, indexy, valuey)" 
                            class="button is-white button-board" 
                            :class="{ 'is-danger': valuey }" 
                            :id="`cell-board-${indexx}-${indexy}`">

                            <i class="fa-solid fa-land-mine-on" ></i>

                        </button>
                        
                        <button 
                            v-if="mode == 'game'" 
                            @click="(e) => verify(e, indexx, indexy)" 
                            class="button is-white" 
                            :id="`cell-game-${indexx}-${indexy}`"
                            :disabled="game_mode != 'Web'">
                            
                            <i class="fa-solid fa-land-mine-on" ></i>

                        </button>
                    </div>
                </div>
            </div>
        </form>
    </section>
</template>