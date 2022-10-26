<template>
    <div class="collection-context">
        <div v-if="data">
          {{data}}
          <hr>
          <h2>Type: {{data.dataType}}</h2>
          <p>Id: {{ data._id }}</p>
          <h4>Name: {{ data.name }}</h4>
          
        </div>
    </div>
</template>

<script>
export default {
  name: "CollectionContextDashboard",
  props: ['data'],
  data() {
    return {
    }
  },
  methods: {

  },
  mounted() {
  },
};
</script>

<style scoped>
</style>