<script setup>

import { onMounted } from 'vue';
import Ram from './store/ram.js'

onMounted(async() => {
    await Ram.fetchMatrixRepresentation()
});

</script>

<template>
    <section class="hero is-link">
        <div class="hero-body has-text-centered">
            <p class="title">MINESWEEPER</p>
        </div>
    </section>
    <div class="container-app">
        <router-view />
    </div>
</template>

