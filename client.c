typedef struct {
    list_head_t list_node;
    char ip[16];
    unsigned short port;
} conn_node_t;


LIST_HEAD(g_conn_list_head);

int load_device_list_config(const char *path)
{
    FILE *fd;
    char *buff;
    int file_size;
    cJSON *root, *device_list, *device, *ip, *port;
    conn_node_t *node;

    fd = fopen(path, 'r');
    if (!fd) {
        printf("open file %s error!\n", path);
        return -1;
    }
    fseek(fd, 0, SEEK_END);
    file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    buff = malloc(file_size + 1);
    if (!buff) {
        printf("no memory!\n");
        fclose(fd);
        return -1;
    }
    fclose(fd);
    memset(buff, 0, sizeof(buff));
    fread(buff, 1, file_size, fd);
    root = cJSON_Parse(buff);
    if (!root) {
        printf("parse config error!\n");
        return -1;
    }
    device_list = cJSON_GetObjectItem(root, "deviceList");
    if (!device_list) {
        printf("config format error!\n");
        cJSON_Delete(root);
        return -1;
    }
    cJSON_ArrayForEach(device, device_list) {
        ip = cJSON_GetObjectItem(device, "ip");
        if (!ip || ip->type != cJSON_String) {
            printf("failed to get ip from config!\n");
            continue;
        }
        port = cJSON_GetObjectItem(device, "port");
        if (!port || port->type != cJSON_Number) {
            printf("failed to get ip from config!\n");
            continue;
        }
        node = (conn_node_t *)malloc(sizeof(conn_node_t));
        if (!node) {
            printf("no memory!\n");
            continue;
        }
        memset(node->ip, 0, sizeof(node->ip));
        snprintf(node->ip, sizeof(node->ip), ip->valuestring);
        node->port = port->valueint;
        list_add(&node->list_node, &g_conn_list_head);
    }
}

int main()
{
   list_for_each_entry() 
}
