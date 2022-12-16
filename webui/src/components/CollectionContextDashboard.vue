<template>
    <div class="collection-context" v-if="colData">
      <div class="context-header" v-if="colData">
        <p class="list-title">{{title}}</p>
        <div class="control-btns" v-if="display == 'readme'">
          <button type="button" class="btn btn-light btn-sm" @click="refresh()">&#x21bb;</button>
          <button type="button" class="btn btn-light btn-sm" @click="openSettings()">&#x26ED;</button>
        </div>
        <div class="control-btns" v-if="display == 'settings'">
          <button type="button" class="btn btn-light btn-sm" @click="openReadme()">Cancel</button>
          <button type="button" class="btn btn-light btn-sm" @click="saveSettings()">Save</button>
        </div>
      </div>

      
      <VueMarkdown class="readme context-body" :source="colData.readme" :options="options" v-if="colData.readme && display == 'readme'" />
      <CollectionSettings ref="collection-settings" class="settings context-body" :colData="colData" v-if="display == 'settings'" @collectionDeleted="collectionDeleted"/>
    </div>
</template>

<script>
import CollectionSettings from '@/components/CollectionSettings.vue';

import VueMarkdown from 'vue-markdown-render';
var hljs = require('highlight.js');
var md = require('markdown-it')({
  html: true,
  linkify: true,
  typographer: true,
  highlight: function (str, lang) {
    if (lang && hljs.getLanguage(lang)) {
      try {
        return '<pre class="hljs codeblock"><code>' +
               hljs.highlight(str, { language: lang, ignoreIllegals: true }).value +
               '</code></pre>';
      } catch (e) {console.log(e);}
    }

    return '<pre class="hljs codeblock"><code>' + md.utils.escapeHtml(str) + '</code></pre>';
  }
});


export default {
  name: "CollectionContextDashboard",
  components: {
    VueMarkdown,
    CollectionSettings
  },
  props: ['colData'],
  data() {
    return {
      options: md,
      display: "readme",
      title: ""
    }
  },
  methods: {
    refresh() {
      this.openReadme();
    },
    openSettings() {
      this.display = "settings";
      this.title = `${this.colData.name} / Settings`;
    },
    async saveSettings() {
      let result = await this.$refs["collection-settings"].save();
      
      if (result) {
        this.$emit("colSettingsChanged", result);
        this.openReadme();
      }
    },
    openReadme() {
      this.display = "readme"
      this.title = `${this.colData.name} / README.md`;
    },
    collectionDeleted() {
      this.$emit("collectionDeleted");
    }
  },
  watch: {
    colData: function(val) {
      this.refresh();
    }
  },
  mounted() {
    this.openReadme();
    this.options = md;
  },
};
</script>

<style scoped>
.collection-context {
  height: 100%;
  display: grid;
  grid-template-rows: auto 1fr;
  grid-template-columns: minmax(0, 1fr);
  grid-template-areas:
    "context-header"
    "context-body";
}

.context-body {
  overflow-y: scroll;
  word-wrap: break-word;
}
.readme {
  text-align: left;
  padding: 10px;
  border-top: 1px solid rgb(211, 211, 211);
  height: 100%;
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
</style>