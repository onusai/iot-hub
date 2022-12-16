<template>
    <div class="collection-settings">
        <div class="properties">
            <h5>Properties</h5>
            <hr>
            <div class="h-field">
                <label class="form-label">Name</label>
                <input ref="form-collection-name" class="form-control form-control-sm" type="text" placeholder="" aria-label=".form-control-sm example">
            </div>
            <hr>
        </div>
        <div class="actions">
            <h5>Actions</h5>
            <hr>
            <div class="h-field">
                <button type="button" class="btn btn-danger btn-sm" @click="deleteCol()">Delete Collection</button>
            </div>
            <hr>

        </div>
        <div class="template">
            <div class="template-header">
                <div class="template-select">
                    <h5>README.md</h5>
                </div>
            </div>
            <codemirror class="template-body" v-model="code" :options="cmOption" />
        </div>
    </div>
</template>

<script>
import axios from 'axios';
import { Codemirror } from 'vue-codemirror';

export default {
  name: "CollectioneSettings",
  props: ['colData'],
  components: {
    Codemirror
  },
  data() {
    return {
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
    setReadme() {
        this.code = this.colData.readme;
    },
    async save() {

        let newData = {
            _id: this.colData._id,
            name: this.$refs["form-collection-name"].value,
            readme: this.code
        };

        try {
            const res = await axios.post('/api/collection/save', newData)
            return newData;
        } catch (err) {
            console.log(err);
            if (err.response) {
                console.log(err.response.data);
            }
            return false;
        }
    },
    update() {
        this.$refs["form-collection-name"].value = this.colData.name;
        if (this.colData.reamdme != "") this.code = this.colData.readme;
    },
    async deleteCol() {
        try {
            const res = await axios.post('/api/collection/delete', {_id: this.colData._id})
            this.$emit("collectionDeleted");
            return true;
        } catch (err) {
            console.log(err);
            if (err.response) {
                console.log(err.response.data);
            }
            this.$emit("collectionDeleted");
            return false;
        }
    }
  },
  watch: {
    colData: function(val) {
        this.update();
    }
  },
  mounted() {
    this.update();
  },
};
</script>

<style scoped>
.collection-settings {
    display: grid;
    grid-template-columns: 1fr 1fr;
    grid-template-rows: minmax(0, 120px) minmax(0, 1fr);
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