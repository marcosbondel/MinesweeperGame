// stores/dataStore.js
import { defineStore } from 'pinia'
import axios from 'axios'

export const useDataStore = defineStore('data', {
    state: () => ({
        bombs: [],
        configuredBombs: false,
        selectedBomb: null,
        loading: false,
        error: null
    }),
    actions: {
        async fetchBombs() {
            this.loading = true
            this.error = null
            try {
                // Simulando una petición a un backend
                const response = await axios.get('https://ejemplo.com/api/bombs')
                this.bombs = response.data
            } catch (e) {
                this.error = 'Error al cargar los datos'
                console.error(e)
            } finally {
                this.loading = false
            }
        },
        async crearPunto(nuevoPunto) {
            this.loading = true
            this.error = null
            try {
              const response = await axios.post('https://ejemplo.com/api/puntos', nuevoPunto)
              this.puntos.push(response.data) // asumimos que el backend devuelve el punto creado
            } catch (e) {
              this.error = 'Error al crear el punto'
              console.error(e)
            } finally {
              this.loading = false
            }
        }
    }
})
