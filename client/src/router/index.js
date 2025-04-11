import { createRouter, createWebHistory } from 'vue-router';
import HomePage from '../pages/Home.vue';
import GamePage from '../pages/Game.vue';
import SettingsPage from '../pages/Settings.vue';

const routes = [
  { path: '/', component: HomePage },
  { path: '/game', component: GamePage },
  { path: '/settings', component: SettingsPage },
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
