<template>

  <header>
    <a href="javascript:void(0)" class="home-btn" @click="redirectHome">
      <img class="home-icon" src="@/assets/icon_full.png" />
    </a>
 
    <div class="page-title">{{ pageTitle }}</div>

    <div class="user-panel">
      <router-link to="/dashboard" class="dashoard-link">
        <p v-if="isLoggedIn && userData != null">{{ userData.email }}</p>
      </router-link>
      <button type="button" class="btn btn-secondary logout-btn btn-sm" v-if="isLoggedIn" @click="logout">Logout</button>
    </div>
  </header>

  <router-view class="router"></router-view>
  
  <footer>
  </footer>

</template>

<script>
import { useStore } from '@/stores/index';
import { storeToRefs } from 'pinia';
import axios from 'axios';

export default {
  components: {},
  methods: {
    redirectHome() {
      this.$router.push("/");
    },
    logout() {
      axios
        .get('/api/auth/logout')
        .then(res => {
          this.store.getLoggedIn();
          this.store.userData = null
          this.$router.push('/');
        })
        .catch(err => {
          if (err.response) {
            this.alertMsg = err.response.data['message'];
          }
        });
    }
  },
  setup() {
    const store = useStore();
    store.loadCookies();
    store.getLoggedIn();
    store.getUserData();
    const { cookies, pageTitle, isLoggedIn, userData} = storeToRefs(store);
    return { store, cookies, pageTitle, isLoggedIn, userData};
  },
}
</script>

<style>
html, body {
  height: 100%;
}

#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;

  display: grid;
  grid-template-columns: 1fr;
  grid-template-rows: 50px minmax(0, 1fr) 25px;
  grid-template-areas: 
    "header"
    "router"
    "footer";
  height: 100%;
  height: 100%;
}

header {
  grid-area: header;
  display: flex;
  align-items: center;
  justify-content: space-between;
  background-color:rgb(33, 35, 44);
}

.router {
  grid-area: router;
  overflow: hidden;
}

footer {
  grid-area: footer;
  background-color:rgb(33, 35, 44);
}

.home-btn {
  min-width: 300px;
  text-align: left;
}

.home-icon {
  max-height: 30px;
  margin-left: 20px;
}

.page-title {
  color: white;
  font-weight: 700;
  font-size: 2em;
}

.user-panel {
  display: flex;
  display: row;
  min-width: 300px;
  text-align: right;
  color: white;
  gap: 10px;
  align-items: center;
  justify-content: right;
}

.dashoard-link:hover, .dashoard-link:visited, .dashoard-link:link, .dashoard-link:active
{
  color: #bfbfbf;
  text-decoration: none;
}

.user-panel p {
  margin: 0;
}

.logout-btn {
  margin-right: 20px;
}
</style>
