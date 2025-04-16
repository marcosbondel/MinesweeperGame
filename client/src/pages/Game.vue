<script setup>

import HeaderComponent from '../components/Header.vue'
import BoardComponent from '../components/Board.vue'
import GoBackComponent from '../components/GoBack.vue';

</script>

<template>
    <div>
        <div class="container-app">
            <HeaderComponent title="Game" headerMode="Game"/>
            <GoBackComponent/>
            <BoardComponent class="mt-6" mode="game"/>
        </div>
    </div>
</template>

