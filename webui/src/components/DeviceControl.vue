<template>
    <div ref="device-control" class="device-control" v-if="devData">
        <div class="tcol" v-for="col_n in tpl.columns" :key=col_n>
          <template v-for="item in tpl.items" :key="devData._id+item.id">
            <TextLabel v-if="(item.column+1)==col_n && item.type=='label'" :devData="devData" :data="item" />
            <MqttButton v-else-if="(item.column+1)==col_n && item.type=='mqtt-btn'" :devData="devData" :data="item" />
            <MqttListener v-else-if="(item.column+1)==col_n && item.type=='mqtt-listener'" :devData="devData" :data="item" />
            <MqttCmdInput v-else-if="(item.column+1)==col_n && item.type=='mqtt-input'" :devData="devData" :data="item" />
          </template>
        </div>
    </div>
</template>

<script>
import { useStore } from '@/stores/index';
import TextLabel from '@/components/control_elements/TextLabel.vue';
import MqttButton from '@/components/control_elements/mqttButton.vue';
import MqttListener from '@/components/control_elements/mqttListener.vue';
import MqttCmdInput from '@/components/control_elements/mqttCmdInput.vue';

export default {
  name: "DeviceRegister",
  props: ['devData'],
  components: {
    TextLabel,
    MqttButton,
    MqttListener,
    MqttCmdInput
  },
  data() {
    return {
    }
  },
  methods: {
    update() {
      this.$refs['device-control'].style['grid-template-columns'] = '1fr '.repeat(this.tpl.columns);
    },
  },
  watch: {
    devData(old_data, new_data) {
      this.update();
      //this.$forceUpdate();
    }
  },
  computed: {
    tpl() {
      return JSON.parse(this.devData.template);
    }
  },
  mounted() {
    this.update();
  },
  setup() {
    const store = useStore();
    return { socket: store.socket };
  }
};
</script>

<style scoped>
.device-control {
    display: grid;
    grid-template-rows: minmax(0, 1fr);
    grid-gap: 10px;
    padding: 10px;
}

.tcol {
  display: flex;
  flex-direction: column;
  gap: 10px;
}
</style>