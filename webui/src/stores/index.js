import axios from "axios";
import { defineStore } from "pinia";
import { useCookies } from "vue3-cookies";
import io from 'socket.io-client';

export const useStore = defineStore("main", {
  state: () => ({
    cookies: null,
    userData: null,
    pageTitle: "",
    isLoggedIn: false,
    socket: io()
  }),
  getters: {
    doubleCount: (state) => state.count * 2,
  },
  actions: {
    increment() {
      this.count++;
    },
    loadCookies() {
      let { cookies } = useCookies();
      this.cookies = cookies;
    },
    getLoggedIn() {
      this.isLoggedIn = this.cookies.get("access_token") != "null";
      return this.isLoggedIn;
    },
    getUserData() {
      axios
        .get("/api/user")
        .then((res) => {
          if (res.data) {
            this.userData = res.data.user;
          }
        })
        .catch((err) => {
          if (err.response) {
            this.alertMsg = err.response.data["message"];
          }
        });
    },
  },
});
