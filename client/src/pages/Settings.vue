<script setup>

import HeaderComponent from '../components/Header.vue'
import GoBackComponent from '../components/GoBack.vue'
import BoardComponent from '../components/Board.vue'

</script>

<template>
    <div>
        <div class="container-app">
            <HeaderComponent title="Settings"/>
            <GoBackComponent/>
            <BoardComponent class="mt-6" mode="settings"/>
        </div>
    </div>
</template>

