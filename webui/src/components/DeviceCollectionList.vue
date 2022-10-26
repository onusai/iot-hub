<template>
    <div class="collections">
        <div class="list-heading">
            <p class="list-title">Collections</p>
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
                <input type="radio" class="btn-check" name="btnradio" :id="'btnradio-'+item._id" autocomplete="off">
                <label class="btn btn-light item-name" :for="'btnradio-'+item._id" @click="itemSelected(item._id)">{{item.name}}</label>
            </div>
            <div class="input-item" v-if="showAddNew">
                <input ref="newItemInput" type="text" class="form-control" placeholder="New Collection Name">
                <button type="button" class="btn btn-primary btn-sm" @click="submitAddNew">Add</button>
                <button type="button" class="btn btn-danger btn-sm" @click="closeAddNew">&#10006;</button>
            </div>
        </div>
    </div>
</template>

<script>
import axios from 'axios';
export default {
    name: "DeviceCollectionList",
    data() {
        return {
            itemList: [],
            showAddNew: false,
            alertMsg: "",
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
            
            let name = this.$refs['newItemInput'].value;
            if (!this.validateInput(name)) return;
            
            axios
                .post('/api/collection/create', {name})
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
        refreshList() {
            this.alertMsg = "";
            axios
                .get('/api/collection/list')
                .then(res => {
                    if (res.data) {
                        this.itemList = res.data;
                        this.itemSelected(this.selectedID)
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
    background-color: #413f4f;
    padding: 10px;
    color: white;
    border-bottom-left-radius: 10px;
    border-bottom-right-radius: 10px;
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
    padding: 10px;
    gap: 5px;

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