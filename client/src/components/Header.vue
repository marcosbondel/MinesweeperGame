<script setup>
import { defineProps, ref, onMounted } from 'vue'
import Ram from '../store/ram.js'

const { title } = defineProps({
    title: {
        type: String,
        default: ''
    },
    headerMode: {
        type: String,
        default: ''
    }
})

const game_mode = ref('')

onMounted(() => {
    game_mode.value = Ram.game_mode
})

</script>

<template>
    <header class="header is-flex is-justify-content-space-between is-align-items-center">
        <nav class="breadcrumb" aria-label="breadcrumbs">
            <ul>
                <li><a>Home</a></li>
                <li><a>{{ title }}</a></li>
            </ul>
        </nav>
        <p class="subtitle has-text-white has-text-weight-bold is-uppercase">
            {{ title }} 
            <span v-if="headerMode === 'Game'">{{ game_mode }}</span>
        </p>
        <p class="subtitle has-text-weight-bold has-text-white">G5</p>
    </header>
</template>