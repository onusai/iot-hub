<template>
    <div class="device-settings">
        <div class="properties">
            <h5>Properties</h5>
            <hr>
            <div class="h-field">
                <label class="form-label">Device Name</label>
                <input ref="form-device-name" class="form-control form-control-sm" type="text" placeholder="" aria-label=".form-control-sm example">
            </div>
            <hr>
            <div class="h-field">
                <label class="form-label">Description</label>
                <input ref="description" class="form-control form-control-sm" type="text" placeholder="" aria-label=".form-control-sm example">
            </div>
            <hr>
        </div>
        <div class="actions">
            <h5>Actions</h5>
            <hr>
            <div class="h-field">
                <button type="button" class="btn btn-danger btn-sm" :disabled='!devData.isRegistered' @click="deregister()">De-register</button>
            </div>
            <hr>
            <div class="h-field">
                <button type="button" class="btn btn-danger btn-sm" @click="deleteDevice()">Delete</button>
            </div>
            <hr>

        </div>
        <div class="template">
            <div class="template-header">
                <div class="template-select">
                    <h5>Template</h5>
                    <div class="dropdown">
                        <button ref="template-dropdown" class="btn btn-secondary dropdown-toggle" type="button" data-bs-toggle="dropdown" aria-expanded="false">
                            Select Device Template
                        </button>
                        <ul class="dropdown-menu dropdown-menu-dark" >
                            <li><button class="dropdown-item btn-sm" type="button" disabled>Simple Switch</button></li>
                            <li><button class="dropdown-item" type="button" disabled>Data Reciever</button></li>
                            <li><hr class="dropdown-divider"></li>
                            <li><button class="dropdown-item" type="button" @click="setTemplate('Custom')">Custom</button></li>
                        </ul>
                    </div>
                </div>
                <button ref="validate-btn" type="button" class="btn" :class="validateBtnClass" @click="validateTemplate()">Validate</button>
            </div>
            <codemirror class="template-body" v-model="code" :options="cmOption" />
        </div>
    </div>
</template>

<script>
import axios from 'axios';
import { Codemirror } from 'vue-codemirror';

let templateCustom = {
  "version": 1,
  "name": "LED Blink",
  "description": "",
  "columns": 1,
  "items": [
    {
      "id": "label_controls",
      "type": "label",
      "column": 0,
      "text": "Press buttons to control LED",
      "styles": {
        "font-size": "2em",
        "font-weight": "bold"
      }
    },
    {
      "id": "label_note",
      "type": "label",
      "column": 0,
      "text": "placeholder for a note",
      "styles": {
        "color": "#979797"
      }
    },
    {
      "id": "btn_on",
      "type": "mqtt-btn",
      "column": 0,
      "text": "[LED] ON",
      "commands": [
        "SIG_ON"
      ],
      "styles": {
        "width": "30%"
      }
    },
    {
      "id": "btn_off",
      "type": "mqtt-btn",
      "column": 0,
      "text": "[LED] OFF",
      "commands": [
        "SIG_OFF"
      ],
      "styles": {
        "width": "30%"
      }
    },
    {
      "id": "btn_status",
      "type": "mqtt-listener",
      "column": 0,
      "text": "LED IS: ",
      "default_value": "N/A",
      "command": "SIG_STATUS",
      "styles": {
        "font-size": "1.4em",
        "color": "#8c8c8c"
      }
    },
    {
      "id": "cmd_input",
      "type": "mqtt-input",
      "column": 0,
      "text": "Set Brightness %",
      "command": "SIG_BRIGHTNESS",
      "styles": {
        "width": "40%"
      }
    }
  ]
}

export default {
  name: "DeviceSettings",
  props: ['devData'],
  components: {
    Codemirror
  },
  data() {
    return {
        validateBtnClass: "btn-secondary",
        code: "{}",
        cmOption: {
          tabSize: 2,
          styleActiveLine: true,
          lineNumbers: true,
          line: true,
          foldGutter: true,
          styleSelectedText: true,
          mode: 'application/json',
          keyMap: "sublime",
          matchBrackets: true,
          showCursorWhenSelecting: true,
          extraKeys: { "Ctrl": "autocomplete" },
          hintOptions:{
            completeSingle: false
          },
        }
    }
  },
  methods: {
    setTemplate(type) {
        this.$refs["template-dropdown"].innerText = type;
        if (type == 'Custom') {
            this.code = JSON.stringify(templateCustom, null, 2);
        }
    },
    async save() {
        if (!this.validateTemplate()) return false;

        let newData = {
            _id: this.devData._id,
            name: this.$refs["form-device-name"].value,
            description: this.$refs["description"].value,
            template: this.code
        };

        try {
            const res = await axios.post('/api/device/save', newData)
            return newData;
        } catch (err) {
            console.log(err);
            if (err.response) {
                console.log(err.response.data);
            }
            return false;
        }
    },
    validateTemplate() {
        try {
            JSON.parse(this.code);
        } catch (e) {
            this.validateBtnClass = "btn-danger";
            return false;
        }
        this.validateBtnClass = "btn-success";
        return true;
    },
    update() {
        this.$refs["form-device-name"].value = this.devData.name;
        this.$refs["description"].value = this.devData.description;
        if (this.devData.template == "none") this.code = "{}";
        else this.code = this.devData.template;
    },
    async deregister() {
        try {
            const res = await axios.post('/api/device/deregister', {_id: this.devData._id})
            this.$emit("refreshDevices");
            return true;
        } catch (err) {
            console.log(err);
            if (err.response) {
                console.log(err.response.data);
            }
            this.$emit("refreshDevices");
            return false;
        }
    },
    async deleteDevice() {
        try {
            const res = await axios.post('/api/device/delete', {_id: this.devData._id})
            this.$emit("refreshDevices", true);
            return true;
        } catch (err) {
            console.log(err);
            if (err.response) {
                console.log(err.response.data);
            }
            this.$emit("refreshDevices", true);
            return false;
        }
    }
  },
  watch: {
    code: function(val) {
        this.validateBtnClass = "btn-secondary";
    },
    devData: function(val) {
        this.update();
    }
  },
  mounted() {
    this.update();
  },
};
</script>

<style scoped>
.device-settings {
    display: grid;
    grid-template-columns: 1fr 1fr;
    grid-template-rows: minmax(0, 180px) minmax(0, 1fr);
    grid-template-areas:
        "properties actions"
        "template template";
    text-align: left;
}

.properties {
    grid-area: properties;
    display: flex;
    flex-direction: column;
    padding: 5px;
    border-right: 1px solid rgb(216, 216, 216);
}

hr {
    margin: 5px 0;
}

.h-field {
    display: flex;
}

.h-field .form-label {
    width: 40%;
    margin: 3px 0 0 0;
}

.actions {
    grid-area: actions;
    display: flex;
    flex-direction: column;
    padding: 5px;
}

.template {
    grid-area: template;
    display: grid;
    grid-template-columns: 1fr;
    grid-template-rows: auto 1fr;
    grid-template-areas:
        "template-header"
        "template-body";
    border-top: 1px solid rgb(216, 216, 216);
}

.template-header {
    grid-area: template-header;
    display: flex;
    justify-content: space-between;
    padding: 10px;
    border-bottom: 1px solid rgb(216, 216, 216);
}

.template-select {
    display: flex;
    align-items: center;
    gap: 10px;
}

.template-select h5 {
    margin: 0;
}

.template-body {
    grid-area: template-body;
}
</style>