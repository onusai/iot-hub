<template>
    <div class="device-context">
        <div v-if="data" class="info-dump">
          {{data}}
          <hr>
          <h2>Type: {{data.dataType}}</h2>
          <p>Id: {{ data._id }}</p>
          <h4>Name: {{ data.name }}</h4>
          <p>isRegistered: {{data.isRegistered}}</p>
          <div v-if="!data.isRegistered">
            <button type="button" v-if="!connectionString" class="btn btn-primary btn-sm" @click="getConnectionString">Request Connection String</button>
            <p class="cstring" v-if="connectionString">connectionString: {{connectionString}}</p>
            <qrcode-vue class="qrcode" :value="connectionString" v-if="connectionString" width="200"></qrcode-vue>
          </div>
        </div>
    </div>
</template>

<script>
import axios from 'axios';
import QrcodeVue from 'qrcode.vue';

export default {
  name: "DeviceContextDashboard",
  props: ['data'],
  components: {
    QrcodeVue
  },
  data() {
    return {
        connectionString: ""
    }
  },
  methods: {
    update() {
        this.connectionString = "";
    },
    getConnectionString() {
            axios
                .post('/api/device/connectionstring', this.data)
                .then(res => {
                    if (res.data) {
                        this.connectionString = res.data.buf;
                    }
                })
                .catch(err => {
                    console.log(err);
                    if (err.response) {
                        this.alertMsg = err.response.data;
                    }
                });
    }
  },
  mounted() {
  },
};
</script>

<style scoped>
.cstring {
  overflow-wrap: anywhere;
  padding: 0 15%;
}
</style>