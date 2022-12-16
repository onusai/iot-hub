<template>
    <p ref="item">{{data.text}}</p>
</template>

<script>
import { useStore } from '@/stores/index';

export default {
  name: "TextLabel",
  props: ['data', 'devData'],
  components: {},
  data() {
    return {}
  },
  methods: {
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
p {
  margin: 0;
  padding: 0;
}
</style>