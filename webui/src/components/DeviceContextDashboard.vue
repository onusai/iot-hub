<template>
    <div class="device-context">
        <div class="context-header" v-if="devData">
            <p class="list-title">{{title}}</p>
            <div class="control-btns" v-if="['control', 'register'].includes(display)">
                <button type="button" class="btn btn-light btn-sm" @click="refreshDevices()">&#x21bb;</button>
                <button type="button" class="btn btn-light btn-sm" @click="openSettings()">&#x26ED;</button>
            </div>
            <div class="control-btns" v-else-if="display == 'settings'">
              <button type="button" class="btn btn-light btn-sm" v-if="devData.template != 'none'" @click="openControl()">Cancel</button>
              <button type="button" class="btn btn-light btn-sm" @click="saveSettings()">Save</button>
            </div>
        </div>
        <DeviceSettings ref="device-settings" class="view-settings context-body" v-if="display=='settings'" :devData="devData" @refreshDevices="refreshDevices"/>
        <DeviceRegister ref="device-register" class="view-register context-body" v-else-if="display=='register'" :devData="devData"/>
        <DeviceControl ref="device-control" class="view-control context-body" v-else-if="display=='control'" :devData="devData"/>
    </div>
</template>

<script>
import DeviceSettings from '@/components/DeviceSettings.vue';
import DeviceRegister from '@/components/DeviceRegister.vue';
import DeviceControl from '@/components/DeviceControl.vue';
import axios from 'axios';

export default {
  name: "DeviceContextDashboard",
  props: ['devData'],
  components: {
    DeviceSettings,
    DeviceRegister,
    DeviceControl
  },
  data() {
    return {
        display: "",
        title: ""
    }
  },
  methods: {
    initDisplay() {
      if (!this.devData) return;
      if (this.devData.template == "none") this.openSettings();
      else if (!this.devData.isRegistered) this.openRegister();
      else this.openControl();
      
    },
    openSettings() {
      this.display = "settings";
      this.title = `${this.devData.name} / Configuration`;
    },
    async saveSettings() {
      let result = await this.$refs["device-settings"].save();
      
      if (result) {
        this.$emit("devSettingsChanged", result);
        this.openControl();
      }
    },
    openControl() {
      if (!this.devData.isRegistered) return this.openRegister();
      this.display = "control";
      this.title = `${this.devData.name} / Control`;
    },
    openRegister() {
      if (this.devData.template == "none") return this.openSettings();
      else if (this.devData.isRegistered) return this.openControl();
      this.display = "register" ;
      this.title = `${this.devData.name} / Register`;
      
    },
    refreshDevices(deselect=false) {
      this.$emit("refreshDevices", deselect);
    },
    getActiveView() {
      if (this.display == "") return;
      if (this.display == "settings") return this.$refs["device-settings"];
      else if (this.display == "register") return this.$refs["device-register"];
      else if (this.display == "control") return this.$refs["device-control"];
    },
  },
  watch: {
    devData: function(val) {
      this.initDisplay();
    }
  },
  mounted() {
    this.initDisplay();
  },
};
</script>

<style scoped>
.device-context {
  height: 100%;
  display: grid;
  grid-template-rows: auto 1fr;
  grid-template-columns: minmax(0, 1fr);
  grid-template-areas:
    "context-header"
    "context-body";
}
.cstring {
  overflow-wrap: anywhere;
  padding: 0 15%;
}

.context-header {
    display: flex;
    justify-content: space-between;
    background-color: #74808a;
    padding: 10px;
    color: white;
    text-align: left;
    align-items: center;
}
.list-title {
    font-size: 1.5em;
    margin: 0;
}
.control-btns {
    display: flex;
    flex-direction: row;
    font-family: "Lucida Sans Unicode";
    gap: 5px;
}

.control-btns button {
    max-height: 30px;
}

.context-body {
  overflow-y: scroll;
  word-wrap: break-word;
}


</style>