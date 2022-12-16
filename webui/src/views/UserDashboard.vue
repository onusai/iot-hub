<template>
    <div class="user-dashbaord">
      <div class="left-pannel">
        <div class="panel-lists">
          <DeviceCollectionList class="dash-item-list" ref="collectionList" @selected="collectionSelected"/>
          <DeviceList class="dash-item-list" ref="deviceList" @selected="deviceSelected"/>
        </div>
        <TerminalWindow ref="terminal" />
      </div>
      <div ref="contex" class="context">
        <CollectionContextDashboard v-if="contextType=='collection'" @colSettingsChanged="colSettingsChanged" :colData="selectedCollection" @collectionDeleted="collectionDeleted"/>
        <DeviceContextDashboard ref="deviceContext" v-else-if="contextType=='device'" @devSettingsChanged="devSettingsChanged" :devData="selectedDevice" @refreshDevices="refreshDevices"/>
        <div class="empty-context" v-else>
          <h1>[ Nothing Selected ]</h1>
        </div>
      </div>
    </div>
</template>

<script>
import { useStore } from '@/stores/index';
import DeviceCollectionList from '@/components/DeviceCollectionList.vue';
import DeviceList from '@/components/DeviceList.vue';
import CollectionContextDashboard from '@/components/CollectionContextDashboard.vue';
import DeviceContextDashboard from '@/components/DeviceContextDashboard.vue';
import TerminalWindow from '@/components/TerminalWindow.vue';
//import io from 'socket.io-client';

const PORT =
  process.env.NODE_ENV == "production"
    ? process.env.PROD_PORT
    : process.env.DEV_PORT;

export default {
  name: "UserDashboard",
  components: {
    DeviceCollectionList,
    DeviceList,
    CollectionContextDashboard,
    DeviceContextDashboard,
    TerminalWindow
  },
  data() {
    return {
      alertMsg: "",
      selectedCollection: null,
      selectedDevice: null,
      selectedMostRecent: null,
      constr: "",
      contextType: ""
    }
  },
  methods: {
    collectionSelected(collection) {
      if (collection.readme == "") collection.readme = "# IoT Device Collection\n< Edit to describe the purpose and usage of this device group >";
      this.selectedCollection = collection;
      this.$refs['deviceList'].changeCollection(collection);
      collection['dataType'] = "collection";
      this.selectedMostRecent = collection;
      this.contextType = "collection";
      if (this.$refs['terminal']) {
        this.$refs['terminal'].open(collection.name, collection);
      }
      this.socket.emit('updateRooms'); // BAD DONT DO THIS
    },
    deviceSelected(device) {
      this.selectedDevice = device;
      device['dataType'] = "device";
      this.selectedMostRecent = device;
      this.contextType = "device";
      if (this.$refs['terminal']) {
        let path = this.selectedCollection.name + "/" + this.selectedDevice.name;
        this.$refs['terminal'].open(path, this.selectedMostRecent)
      }
    },
    refreshDevices(deselect) {
      this.$refs["deviceList"].refreshList();
      if (deselect) {
        this.collectionSelected(this.selectedCollection);
      }
    },
    devSettingsChanged(newData) {
      this.selectedDevice.name = newData.name;
      this.selectedDevice.description = newData.description;
      this.selectedDevice.template = newData.template;
      this.selectedMostRecent = this.selectedDevice;
    },
    colSettingsChanged(newData) {
      this.selectedCollection.name = newData.name;
      this.selectedCollection.readme = newData.readme;
      this.selectedMostRecent = this.selectedCollection;
    },
    collectionDeleted() {
      this.selectedCollection = null;
      this.selectedDevice = null;
      this.selectedMostRecent = null;
      this.contextType = "";
      this.$refs["collectionList"].refreshList();
    }
  },
  mounted() {
    let store = useStore();
    store.pageTitle = "Dashboard";
    if (store.isLoggedIn) {
      this.socket.emit('mqttListen', store.cookies.get("access_token"));
    }
  },
  setup() {
    const store = useStore();
    store.getLoggedIn();
    store.getUserData();
    return { store, socket:store.socket };
  }
};
</script>

<style scoped>
.user-dashbaord {
  display: grid;
  width: 100%;
  height: 100%;
  grid-template-columns: 2fr 3fr;
  grid-template-rows: minmax(0, 1fr);
  grid-template-areas: "left-pannel contex";
}

.left-pannel {
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.panel-lists {
  display: flex;
  flex-grow: 1;
  overflow: hidden;
}

.collections {
  background-color:#727c8e;
  width: 100%;
}

.devices {
  background-color: #bbc4ce;
  width: 100%;
  overflow: hidden;
}

.dash-item-list {
  overflow: hidden;
}

.context {
  background-color:rgb(251, 251, 251);
  width: 100%;
  height: 100%;
}

.empty-context {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100%;
  color: rgb(187, 187, 187);
}
</style>