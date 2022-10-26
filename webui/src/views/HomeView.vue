<template>
  <div class="home">
    <div class="info-section">
      <h1>IoT Hub</h1>
      <p>Framework for developing and managing IoT devices </p>
    </div>

    <LoginForm class="login-section" />
  </div>
</template>

<script>
import LoginForm from '@/components/LoginForm.vue';
import { useStore } from '@/stores';

export default {
  name: "HomeView",
  components: {
    LoginForm,
  },
  mounted() {
    let store = useStore();
    store.pageTitle = "";
  }
};
</script>


<style scoped>

.home {
  display: grid;
  grid-template-columns: 2fr 1fr;
  grid-template-rows: 1fr;
  grid-template-areas: "info login";
}

.info-section {
  grid-area: info;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  font-size: 1.5em;
  /* border: 1px solid rgb(175, 175, 175); */
  background: rgb(199,197,215);
  background: linear-gradient(112deg, rgba(199,197,215,1) 0%, rgba(255,255,255,1) 40%);
}

.info-section h1 {
  font-size: 5em;
  font-weight: 700;
}

.login-section {
  grid-area: login;
  border-left: 1px solid rgb(175, 175, 175);
}
</style>