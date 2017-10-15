<template>
  <div>
    <spider-header id="head" :isConnected="connected"></spider-header>
    <div id="app">
     <div v-if="!connected">
       <connection @connected="connectedToServer"></connection>
      </div>
      <div v-if="connected">
        <spider-server-infos :serverAddr="serveraddr" :connected="connected"></spider-server-infos>
        <spider-clients-infos :serverAddr="serveraddr"></spider-clients-infos>
      </div>
    </div>
  </div>
</template>

<script>
import Connection from './Connection/Connection.vue';
import SpiderHeader from './Spider-Header/SpiderHeader.vue';
import SpiderServerInfos from './Spider-Server-Infos/SpiderServerInfos.vue';
import SpiderClientsInfos from './Spider-Clients-Infos/SpiderClientsInfos.vue';

export default {
  name: 'app',
  components: { Connection, SpiderHeader, SpiderServerInfos, SpiderClientsInfos },
  data () {
    return {
      connected: false,
        serveraddr: ''
    }
  },
  methods: {
    connectedToServer(serveraddr) {
        this.serveraddr = serveraddr;
        console.log("serverAddr in parent : ", serveraddr);
        this.connected = true;
    }
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 20px;
  max-width: 100%;
  overflow-x: hidden;
}

#head {
  max-width: 100%;
  overflow-x: hidden;
}

h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b983;
}
</style>
