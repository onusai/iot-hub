<template>
    <div class="terminal open">
        <div class="terminal-head">
            Terminal
        </div>
        <div ref="tbody" class="terminal-body">
            <div class="t-line" v-for="line in log[path]" v-bind:key="line.timestamp">{{line.text}}</div>
        </div>
        <div class="terminal-input">
            <p>
                <span v-if="isLoggedIn && userData != null" style="">{{ userData.email.split('@')[0] }}</span>@iothub:{{path}}~$</p>
            <input ref="tinput" type="text" v-on:keyup.enter="onEnter"/>
        </div>
        

    </div>
</template>

<script>
import { useStore } from '@/stores/index';
import { storeToRefs } from 'pinia';

export default {
    name: "TerminalWindow",
    data() {
        return {
            path: "",
            log: {"":[{text:"IoT Hub v0.1 - Type 'help' to show commands", timestamp:""}]},
            item: null
        }
    },
    methods: {
        open(path, item) {
            this.item = item;
            this.path = path;
            if (!(path in this.log)) {
                this.log[path] = [];
            }
        },
        onEnter() {
            let tinput = this.$refs["tinput"];
            let value = tinput.value;
            this.log[this.path].push({text: "> " + value, timestamp: new Date().getTime()});
            
            if (value == "clear")
                this.log[this.path] = [];
            if (value == "help") {
                this.print("Commands:")
                this.print("-  clear : clears terminal")
            }
            else if (this.item && this.item.dataType == "device") {
                this.socket.emit("mqttCMD", {topic: `${this.item.deviceCollection}/${this.item._id}`, message: value});
            }
            tinput.value = "";
            this.scrollLogsToBottom();
            
        },
        print(text) {
            this.log[this.path].push({text: text, timestamp: new Date().getTime()});
            this.scrollLogsToBottom();
        },
        scrollLogsToBottom() {
            let tbody = this.$refs["tbody"];
            tbody.scrollTop = tbody.scrollHeight;
        },
        attachIO(socket) {
            this.socket = socket;
            this.socket.on('mqttData', (data) => {
                console.log(data);
                this.print(data.message);
            });
        }
    },
    mounted() {
    },
    setup() {
        const store = useStore();
        store.getLoggedIn();
        store.getUserData();
        const { isLoggedIn, userData} = storeToRefs(store);
        return { store, isLoggedIn, userData};
    },
}
</script>

<style scoped>

.terminal {
    font-family: "Courier New", Courier, monospace;
    text-align: left;
    color: #acb9c6;
    font-size: 14px;
}

.open {
    display: grid;
    grid-template-columns: 1fr;
    grid-template-rows: 25px 250px 30px;
    grid-template-areas:
        "terminal-head"
        "terminal-body"
        "terminal-input";
}

.terminal-head {
    background-color: #413f4f;
    padding-left: 10px;
}

.terminal-body {
    background-color: #171717;
    padding-left: 10px;
    overflow: scroll;
}

.terminal-input {
    display: flex;
    background-color: black;
}

.terminal-input p {
    margin: 0;
    padding-left: 10px;
    padding-top: 4px;
}
.terminal-input input {
    margin: 0;
    flex-grow: 1;
    background-color: black;
    outline: none;
    border: none;
    padding-left: 5px;
    color: #acb9c6;
}
</style>