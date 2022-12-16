<template>
    <div class="devices">
        <div class="list-heading">
            <p class="list-title">Devices</p>
            <div class="control-btns">
                <button type="button" class="btn btn-light btn-sm" @click="showAddNew = true">+</button>
                <button type="button" class="btn btn-light btn-sm" @click="refreshList()">&#x21bb;</button>
            </div>
        </div>
        <div class="alert alert-danger" role="alert" v-if="alertMsg!=''">
            {{alertMsg}}
        </div>
        <div class="item-list">
            <div class="list-item" v-for="item in itemList" v-bind:key="item._id">
                <input type="radio" class="btn-check" name="btnradio-device" :id="'btnradio-device-'+item._id" autocomplete="off">
                <label class="btn btn-secondary btn-sm item-name" :for="'btnradio-device-'+item._id" @click="itemSelected(item._id)">{{item.name}}</label>
            </div>
            <div class="input-item" v-if="showAddNew">
                <input ref="newItemInput" type="text" class="form-control" placeholder="New Device Name">
                <button type="button" class="btn btn-primary btn-sm" @click="submitAddNew">Add</button>
                <button type="button" class="btn btn-danger btn-sm" @click="closeAddNew">&#10006;</button>
            </div>
        </div>
    </div>
</template>

<script>
import axios from 'axios';
export default {
    name: "DeviceList",
    data() {
        return {
            itemList: [],
            showAddNew: false,
            alertMsg: "",
            collection: {},
            selectedID: null
        }
    },
    methods: {
        itemSelected(id) {
            this.alertMsg = "";
            let item = this.itemList.forEach(e => {
                if (e._id == id) {
                    //console.log(e._id);
                    this.selectedID = id;
                    return this.$emit('selected', e);
                }
            })
        },
        submitAddNew() {

            this.alertMsg = "";
            this.showAddNew = false;
            
            if (this.collection == null) return;

            let name = this.$refs['newItemInput'].value;
            if (!this.validateInput(name)) return;
            
            //console.log(this.collection._id)
            axios
                .post('/api/device/create', {name, deviceCollection: this.collection._id, template:"none"})
                .then(res => {
                    this.refreshList();
                })
                .catch(err => {
                    console.log(err);
                    if (err.response) {
                        this.alertMsg = err.response.data;
                    }
                });
        },
        closeAddNew() {
            this.alertMsg = "";
            this.showAddNew = false;
        },
        changeCollection(collection) {
            this.collection = collection;
            this.refreshList(false)
        },
        refreshList(select=true) {
            this.alertMsg = "";//this.collection._id
            if (this.collection == null) return;
            
            axios
                .post('/api/device/list', {deviceCollection: this.collection._id})
                .then(res => {
                    if (res.data) {
                        this.itemList = res.data;
                        if (select) this.itemSelected(this.selectedID)
                    }
                })
                .catch(err => {
                    if (err.response) {
                        this.alertMsg = err.response.data['message'];
                    }
                });
        },
        validateInput(text) {
            let valid = true;
            if (text == "") valid = false;
            return valid;
        }
    },
    mounted() {
        this.refreshList();
    },
    setup() {
    },
    components: {  }
}
</script>

<style scoped>

.btn {
    width: 100%;
}

.list-heading {
    display: flex;
    justify-content: space-between;
    background-color: #586473;
    padding: 10px;
    color: white;
    text-align: left;
    align-items: center;
}

.list-title {
    font-size: 1.5em;
    margin: 0;
}

.item-list {
    display: flex;
    flex-direction: column;
    gap: 5px;
    width: 100%;
    height: 100%;
    overflow-y: scroll;
    box-sizing: content-box;
    padding: 8px 17px 8px 8px;
}

.input-item {
    display: flex;
    flex-direction: row;
    gap: 5px;
}

.input-item button {
    max-width: fit-content;
    border-radius: 7px;
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

.item-name {
    text-align: left;
}
</style>