<template>
    <div class="pos-f-t">
        <nav class="navbar navbar-inverse bg-inverse">
            <div class="row">
                <div class="col-md-auto">
                    <button class="navbar-toggler" type="button" data-toggle="collapse"
                            data-target="#navbarToggleExternalContent" aria-controls="navbarToggleExternalContent" aria-expanded="false" aria-label="Toggle navigation">
                        <span class="navbar-toggler-icon"></span>
                    </button>
                </div>
                <div class="col">
                    <label class="spyderTitle">Spyder_v1.0</label>
                </div>
                <div v-if="isConnected" class="col-md-auto">
                    <button @click="logout" class="btn btn-outline-danger my-2 my-sm-0">Log out</button>
                </div>
            </div>
        </nav>
        <div class="collapse" id="navbarToggleExternalContent">
            <div class="bg-inverse p-4">
                <h4 class="text-white">Remain hidden <i class="fa fa-user-secret"></i></h4>
                <span class="text-muted">By Bache, Khsime, Lambert & Troncy</span>
            </div>
        </div>

    </div>
</template>

<script>
    export default {
        name: 'SpiderHeader',
        props: [ 'isConnected' ],
        methods : {
            logout() {
                this.$parent.connected = false;
            }
        }
    }
</script>

<style>
    .spyderTitle {
        color: ghostwhite;
    }
</style>

