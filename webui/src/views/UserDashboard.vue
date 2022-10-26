<template>
    <div class="user-dashbaord">
      <div class="left-pannel">
        <div class="panel-lists">
          <DeviceCollectionList @selected="collectionSelected"/>
          <DeviceList ref="deviceList" @selected="deviceSelected"/>
        </div>
        <TerminalWindow ref="terminal"/>
      </div>
      <div ref="contex" class="contex">
        <CollectionContextDashboard v-if="contextType=='collection'" :data="selectedCollection"/>
        <DeviceContextDashboard ref="deviceContext" v-if="contextType=='device'" :data="selectedDevice"/>
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
import io from 'socket.io-client';

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
      contextType: "",
      socket: io(`http://localhost:8081`)
    }
  },
  methods: {
    collectionSelected(collection) {
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
      if (this.$refs['deviceContext'])
        this.$refs['deviceContext'].update();
      if (this.$refs['terminal']) {
        let path = this.selectedCollection.name + "/" + this.selectedDevice.name;
        this.$refs['terminal'].open(path, this.selectedMostRecent)
      }
        
    }
  },
  mounted() {
    let store = useStore();
    store.pageTitle = "Dashboard";
    if (store.isLoggedIn) {
      this.socket.emit('mqttListen', store.cookies.get("access_token"));
    }
    this.$refs['terminal'].attachIO(this.socket);

  },
  setup() {
    const store = useStore();
    store.getLoggedIn();
    store.getUserData();
    return { store };
  }
};
</script>

<style scoped>
.user-dashbaord {
  display: grid;
  grid-template-columns: 2fr 3fr;
  grid-template-rows: 1fr;
  grid-template-areas: "left-pannel contex";
  min-height: 100%;
}

.left-pannel {
  display: flex;
  flex-direction: column;
}

.panel-lists {
  display: flex;
  flex-grow: 1;
}

.collections {
  background-color:#727b8e;
  width: 100%;
}

.devices {
  background-color: lightgray;
  width: 100%;
}

.context {
  background-color: white;
}
</style>