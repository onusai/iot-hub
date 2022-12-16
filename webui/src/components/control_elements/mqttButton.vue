<template>
    <div>
        <button ref="item" type="button" class="btn btn-primary" @click="send">{{ data.text }}</button>
    </div>
    
</template>

<script>
import { useStore } from '@/stores/index';

export default {
  name: "MqttButton",
  props: ['data', 'devData'],
  components: {},
  data() {
    return {}
  },
  methods: {
    send() {
        for (let cmd of this.data.commands) {
            this.socket.emit("mqttCMD", {topic: `${this.devData.deviceCollection}/${this.devData._id}`, message: cmd});
        }
    }
  },
  watch: {
  },
  computed: {
  },
  mounted() {
    if ('styles' in this.data) {
      for (const [key, value] of Object.entries(this.data.styles)) {
        this.$refs["item"].style[key] = value;
      }
    }
  },
  setup() {
    const store = useStore();
    return { socket: store.socket };
  }
};
</script>

<style scoped>
</style>