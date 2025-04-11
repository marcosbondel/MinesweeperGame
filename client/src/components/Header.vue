<script setup>
import { defineProps, ref } from 'vue'

const { title } = defineProps({
    title: {
        type: String,
        default: ''
    }
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
        <p class="subtitle has-text-white has-text-weight-bold is-uppercase">{{ title }}</p>
        <p class="subtitle has-text-weight-bold has-text-white">G5</p>
    </header>
</template>