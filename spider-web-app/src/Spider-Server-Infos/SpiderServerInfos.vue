<template>
    <div class="row">
        <div class="col">
            <label>Server status:
                <label style="cursor:default" v-if="isOn" class="btn btn-block btn-success active">
                    <input type="checkbox" name="status" checked autocomplete="off"> On
                </label>
                <label style="cursor:default" v-if="!isOn" class="btn btn-block btn-danger active">
                    <input type="checkbox" name="status" checked autocomplete="off"> Off
                </label>
            </label>
        </div>
        <div class="col">
            <label>Server address: {{addr}}</label>
        </div>
        <div class="col">
            <label> {{pluralizeClient(nbConnected)}} connected</label>
        </div>
    </div>
</template>

<script>

    export default {
        name: 'SpiderServerInfos',
        props: [ 'serverAddr', 'connected' ],
        data () {
            return {
                isOn: false,
                isEdit: false,
                nbConnected: 0,
                addr: '',
                isConnected: false,
                interval: null
            }
        },
        beforeMount() {
            this.addr = this.serverAddr;
            this.isConnected = this.connected;
        },
        mounted () {
            this.getNb('nbConnected');
        },
        methods : {
            getNb(nb) {
                this.$http.get('http://' + this.serverAddr + ':5345/nb').then((response) =>
                    response.json()).then(json => {
                    this.nbConnected = json.nbClients;
                    console.log('success ', json);
                    this.isOn = true;
                    return this.nbConnected;
                }, (response) => {
                    this.isOn = false;
                    this.nbConnected = 0;
                    console.log('failed', response);
                });
            },
            pluralizeClient(count) {
                if (count === 0) {
                    return 'no client'
                } else if (count === 1) {
                    return '1 client'
                } else {
                    return count + ' clients'
                }
            }
        }
    }

</script>

<style>
    .btn input {
        display: none;
    }

    .col {
        margin: auto;
    }
</style>
