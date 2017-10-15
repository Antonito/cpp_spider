<template>
    <div>
        <div v-show="isValid" class="alert alert-danger" role="alert" id="alert-ip">
            <a href="#" class="alert-link">Invalid IP Address</a>
        </div>
        <div class="log-container">
         <p>Please connect before access to server</p><br>
            <div style="margin: auto;display: -webkit-inline-box;">
                <div class="form-group mx-sm-3">
                    <label for="server-addr" class="sr-only">Server ip</label>
                    <input type="text" class="form-control" id="server-addr" placeholder="Server's ip..">
                </div>
                <button type="submit" class="btn btn-primary" @click="connectToServer">Connect to Server</button>
            </div>
        </div>
    </div>
</template>

<script>
    export default {
        name: 'Connection',
        data () {
            return {
                isValid: false
            }
        },
        methods: {
            connectToServer() {

                this.$http.get('http://176.31.101.117:5345/').then((response) => {
                    console.log('success', response);
                }, (response) => {
                    console.log('failed', response);
                });

                let serveraddr = document.getElementById('server-addr').value;

                if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(serveraddr)) {
                    // Try connection here when server is ok
                    this.$emit('connected', serveraddr);

                } else {
                    this.isValid = true;
                    $("#alert-ip").show( function () {
                        $("#alert-ip").fadeTo(2000, 500).slideUp(500, function() {
                            $("#alert-ip").slideUp(500);
                        });
                    });
                    this.isValid = false;
                }
            }
        }
    }

</script>

<style>
    .alert-danger {
        width: 25%;
        margin: auto;
    }

    .log-container {
        margin: auto;
    }
    .btn:hover {
        cursor: pointer;
    }
</style>

