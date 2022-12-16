<template>
    <p ref="item">{{data.text + value}}</p>
</template>

<script>
import { useStore } from '@/stores/index';

export default {
  name: "MqttListener",
  props: ['data', 'devData'],
  components: {},
  data() {
    return {
        value: ""
    }
  },
  methods: {
  },
  watch: {
  },
  computed: {
  },
  mounted() {
    this.value = this.data.default_value;
    if ('styles' in this.data) {
      for (const [key, value] of Object.entries(this.data.styles)) {
        this.$refs["item"].style[key] = value;
      }
    }
    this.socket.on('mqttData', (mdata) => {
        if (mdata.deviceID == this.devData._id) {
            let idx = mdata.message.indexOf("/");
            let cmd = mdata.message.substring(0, idx);
            let message = mdata.message.substring(idx+1, mdata.message.length);
            if (cmd == this.data.command) {
                this.value = message;
            }
        }
    });
  },
  setup() {
    const store = useStore();
    return { socket: store.socket };
  }
};
</script>

<style scoped>
p {
  margin: 0;
  padding: 0;
}
</style>