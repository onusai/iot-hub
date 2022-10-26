import { createApp } from "vue";
import App from "./App.vue";
import router from "./router";

import "bootstrap/dist/css/bootstrap.min.css";
import "bootstrap";

import { createPinia } from "pinia";
const pinia = createPinia();

// import VueSocketIO from "vue-socket.io";
// import socketio from "socket.io-client";
// export const SocketInstance = socketio(
//   `http://localhost:${process.env.DEV_PORT}`
// );

//  .use(VueSocketIO, SocketInstance)

createApp(App).use(pinia).use(router).mount("#app");
