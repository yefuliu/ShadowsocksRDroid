#include <string.h>
#include <stdlib.h>

#include "obfs.h"

#include "obfsutil.c"
#include "crc32.c"
#include "http_simple.c"
#include "verify.c"
#include "auth.c"



void * init_data() {
    return "";
}

obfs * new_obfs() {
    obfs * self = (obfs*)malloc(sizeof(obfs));
    self->l_data = NULL;
    return self;
}

void set_server_info(obfs *self, server_info *server) {
    memmove(&self->server, server, sizeof(server_info));
}

void dispose_obfs(obfs *self) {
    free(self);
}

obfs_class * new_obfs_class(char *plugin_name)
{
    if (plugin_name == NULL)
        return NULL;
    if (strcmp(plugin_name, "origin") == 0)
        return NULL;
    if (strcmp(plugin_name, "plain") == 0)
        return NULL;
    init_crc32_table();
    if (strcmp(plugin_name, "http_simple") == 0) {
        obfs_class * plugin = (obfs_class*)malloc(sizeof(obfs));
        plugin->init_data = init_data;
        plugin->new_obfs = http_simple_new_obfs;
        plugin->set_server_info = set_server_info;
        plugin->dispose = http_simple_dispose;

        plugin->client_encode = http_simple_client_encode;
        plugin->client_decode = http_simple_client_decode;

        return plugin;
    } else if (strcmp(plugin_name, "verify_simple") == 0) {
        obfs_class * plugin = (obfs_class*)malloc(sizeof(obfs));
        plugin->init_data = init_data;
        plugin->new_obfs = verify_simple_new_obfs;
        plugin->set_server_info = set_server_info;
        plugin->dispose = verify_simple_dispose;

        plugin->client_pre_encrypt = verify_simple_client_pre_encrypt;
        plugin->client_post_decrypt = verify_simple_client_post_decrypt;

        return plugin;
    } else if (strcmp(plugin_name, "auth_simple") == 0) {
        obfs_class * plugin = (obfs_class*)malloc(sizeof(obfs));
        plugin->init_data = auth_simple_init_data;
        plugin->new_obfs = auth_simple_new_obfs;
        plugin->set_server_info = set_server_info;
        plugin->dispose = auth_simple_dispose;

        plugin->client_pre_encrypt = auth_simple_client_pre_encrypt;
        plugin->client_post_decrypt = auth_simple_client_post_decrypt;

        return plugin;
    }
    return NULL;
}

void free_obfs_class(obfs_class *plugin) {
    free(plugin);
}

