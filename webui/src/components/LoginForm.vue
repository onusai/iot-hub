<template>
  <div class="login-section" >
    <div class="login-select">
      <div class="btn-group" role="group" aria-label="Basic example">
        <input type="radio" class="btn-check" name="btnradio" id="btnradio1" autocomplete="off" checked>
        <label class="btn btn-outline-secondary" for="btnradio1" @click="switchView('login')">Login</label>

        <input type="radio" class="btn-check" name="btnradio" id="btnradio2" autocomplete="off">
        <label class="btn btn-outline-secondary" for="btnradio2" @click="switchView('signup')">Create Account</label>
    </div>
    </div>
    <form class="login-form" >
      <div class="mb-3">
        <label for="InputEmail1" class="form-label">Email address</label>
        <input ref="InputEmail1" type="email" class="form-control" id="InputEmail1" aria-describedby="emailHelp">
      </div>
      <div class="mb-3">
        <label for="InputPassword1" class="form-label">Password</label>
        <input ref="InputPassword1" type="password" class="form-control" id="InputPassword1">
      </div>
      <div class="mb-3" v-if="view=='signup'">
        <label for="InputPassword2" class="form-label">Confirm Password</label>
        <input ref="InputPassword2" type="password" class="form-control" id="InputPassword2">
      </div>
      <div class="alert alert-danger" role="alert" v-if="alertMsg!=''">
        {{alertMsg}}
      </div>
      <button type="button" class="btn btn-primary" v-if="view=='login'" @click="login">Login</button>
      <button type="button" class="btn btn-primary" v-if="view=='signup'" @click="signup">Register</button>
    </form>
  </div>
</template>

<script>
import { assertExpressionStatement } from '@babel/types';
import axios from 'axios';

export default {
  name: "LoginForm",
  data() {
    return {
      view: "login",
      alertMsg: ""
    }
    
  },
  methods: {
    switchView(view) {
      this.view = view;
      this.alertMsg = "";
    },
    signup() {
      if (!this.validateFields()) return;

      let email = this.$refs["InputEmail1"].value;
      let password = this.$refs["InputPassword1"].value;

      axios
        .post('/api/auth/signup', null, { params: {email, password}})
        .then(res => {
          this.login();
        })
        .catch(err => {
          if (err.response) {
            this.alertMsg = err.response.data['message'];
          }
        });
    },
    login() {
      if (!this.validateFields()) return;

      let email = this.$refs["InputEmail1"].value;
      let password = this.$refs["InputPassword1"].value;

      axios
        .post('/api/auth/login', null, { params: {email, password}})
        .then(res => {
          this.$router.push('/dashboard');
        })
        .catch(err => {
          if (err.response) {
            this.alertMsg = err.response.data['message'];
          }
        });
    },
    validateFields() {
      this.alertMsg = "";

      let email = this.$refs["InputEmail1"].value;
      let password = this.$refs["InputPassword1"].value;

      let isEmailValid = this.validateEmail(email);
      let isPassValid = this.validatePassword(password);

      if (!isEmailValid) {
        this.alertMsg = "Invalid Email";
      }
      else if (!isPassValid) {
        this.alertMsg = "Invalid Password. Must contain at least 8 characters, including digit(s), lowercase, and uppercase letter(s)."
      }
      else if (this.view == "signup") {
        let password2 = this.$refs["InputPassword2"].value;
        if (password == password2) return true;
        this.alertMsg = "Password does not match"
      } else {
        return true;
      }
      return false;
    },
    validateEmail(email) {
      return email.toLowerCase().match(/^[^\s@]+@[^\s@]+\.[^\s@]+$/) != null;
    },
    validatePassword(pass) {
      /* /^
        (?=.*\d)          // should contain at least one digit
        (?=.*[a-z])       // should contain at least one lower case
        (?=.*[A-Z])       // should contain at least one upper case
        [a-zA-Z0-9]{8,}   // should contain at least 8 from the mentioned characters
        $/
      */
      return pass.match(/^(?=.*\d)(?=.*[a-z])(?=.*[A-Z])[0-9a-zA-Z]{8,}$/) != null;
    }
  },
}
</script>

<style scoped>
.login-section {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}

.login-form {
  padding: 20px;
  border: 1px solid rgb(175, 175, 175);
  border-radius: 20px;
  width: 60%;
  max-width: 300px;
}

.login-select {
  padding-bottom: 5px;
}
</style>