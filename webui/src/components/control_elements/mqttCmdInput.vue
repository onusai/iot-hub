<template>
    <div class="item-box">
        <div ref="item" class="input-group">
            <span class="input-group-text" id="basic-addon3">{{data.text}}</span>
            <input ref="cmd-input" type="text" class="form-control" id="basic-url" aria-describedby="basic-addon3">
            <button class="btn btn-outline-secondary" type="button" id="button-addon3" @click="send">Send</button>
        </div>
    </div>
</template>

<script>
import { useStore } from '@/stores/index';

export default {
  name: "MqttCmdInput",
  props: ['data', 'devData'],
  components: {},
  data() {
    return {
    }
  },
  methods: {
    send() {
        let cmd = this.$refs['cmd-input'].value;
        this.socket.emit("mqttCMD", {topic: `${this.devData.deviceCollection}/${this.devData._id}`, message: this.data.command+"/"+cmd});
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
.item-box {
    display: flex;
    justify-content: center;
}
</style>