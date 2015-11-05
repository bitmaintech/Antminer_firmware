#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <semaphore.h>
#include <string.h>
#include "debug.h"
#include "get_ip.h"
#include "aes.h"
#include "cgminer_api.h"
#include "construct_ip_struct.h"
#include "ping.h"
#include "cjson.h"

#define SEVER_IP "d-ddos.antpool.com"
#define INTERVAL_FOR_CHECK_DDOS 90
#define API_HOST "127.0.0.1"
#define API_PORT 4028

#define GET_POOLS_CMD "{\"command\":\"pools\"}"
#define ADD_POOL_CMD "{\"command\":\"addpool\",\"parameter\":\"%s:%s,%s,%s\"}"
#define REMOVE_POOL_CMD "{\"command\":\"removepool\",\"parameter\":%d}"
#define SET_PRIORITY_CMD "{\"command\":\"poolpriority\",\"parameter\":\"%s\"}"

#define INFO_HEAD "Added pool "

#define list_for_each(pos,head) \
        for (pos = (head); pos != NULL; pos = pos->next)

#define free_list(list,free_list) \
        while(list)\
        {\
            free_list = list;\
            list = list->next;\
            free(free_list);\
        }\
        list = NULL;\
        free_list = NULL;

#define free_list_cpools(list,free_list) \
        while(list)\
        {\
            free_list = list;\
            list = list->next;\
            free(free_list->pool);\
            free(free_list->status);\
            free(free_list->user);\
            free(free_list);\
        }\
        list = NULL;\
        free_list = NULL;

#define free_list_node(list,free_list) \
        while(list)\
        {\
            free_list = list;\
            list = list->next;\
            free(free_list->ip);\
            free(free_list->port);\
            free(free_list);\
        }\
        list = NULL;\
        free_list = NULL;

#define free_list_server(list,free_list) \
        while(list)\
        {\
            free_list = list;\
            list = list->next;\
            free(free_list->address);\
            free(free_list);\
        }\
        list = NULL;\
        free_list = NULL;

int for_tag = 0;
int pool = -1;
char *ip = NULL;
char *ip_server = NULL;
char *back_server = NULL;
bool ddos_flag = true;

typedef struct server
{
    char *address;
    struct server *next;
} server;

server *server_list = NULL;
/**
 * check if command's response is correct
 */
bool check_api_status(cJSON *my_json_string)
{
    bool ret = false;
    cJSON *s = cJSON_GetArrayItem(my_json_string, 0);
    char *ss = cJSON_GetObjectItem(s, "STATUS")->valuestring;
    ret = strcmp(ss, "S") == 0 ? true : false;
    return ret;
}

/**
 * exec cgminer-api and return the response
 */
char *get_cgminer_api(char *command)
{
    return callapi(command, API_HOST, API_PORT);;
}

/**
 * get the pool setting on miner and create pools struct
 */
void get_pools_info(pools **c_pools)
{
    DEBUG_printf("Function start %s\n", "get_pools_info");
    DEBUG_printf("%s", "checking...\n");
    char *pool_json = get_cgminer_api(GET_POOLS_CMD);
    if (pool_json != NULL)
    {
        cJSON *root = cJSON_Parse(pool_json);
        cJSON *status_info = cJSON_GetObjectItem(root, "STATUS");

        if (check_api_status(status_info))
        {
            cJSON *pools_info = cJSON_GetObjectItem(root, "POOLS");
            int a = cJSON_GetArraySize(pools_info);
            pools *pool_h, *pool_t;
            pool_h = pool_t = NULL;
            if (a > 0)
            {
                pool_h = (pools *) calloc(1, sizeof(pools));
            }
            for (for_tag = 0; for_tag < a; for_tag++)
            {
                cJSON *pool = cJSON_GetArrayItem(pools_info, for_tag);

                pool_h->pool = cp_string(pool_h->pool, cJSON_GetObjectItem(pool, "URL")->valuestring);
                pool_h->status = cp_string(pool_h->status, cJSON_GetObjectItem(pool, "Status")->valuestring);
                pool_h->user = cp_string(pool_h->user, cJSON_GetObjectItem(pool, "User")->valuestring);
                pool_h->Priority = cJSON_GetObjectItem(pool, "Priority")->valueint;
                pool_h->next = NULL;
                if (NULL == *c_pools)
                {
                    *c_pools = pool_h;
                } else
                {
                    pool_t->next = pool_h;
                }
                pool_t = pool_h;
                pool_h = (pools *) calloc(1, sizeof(pools));
            }
            if (a > 0)
            {
                free(pool_h);
            }
            pool_t = NULL;
            pool_h = NULL;
        }
        cJSON_Delete(root);
    }
    free(pool_json);
    DEBUG_printf("Function end %s\n", "get_pools_info");
}

void get_backup_ip_string(char *e_list, ip_string **ip_string_e)
{
    DEBUG_printf("Function start %s\n", "get_backup_ip_string");
    char *content = NULL;
    ip_string *tmp = (ip_string *) calloc(1, sizeof(ip_string));
    if (e_list != NULL)
    {
        content = strchr(e_list, ',');
        if (content != NULL)
            *(content++) = '\0';
        tmp->len = atoi(e_list);
        tmp->encrypt_string = hex_to_uc(content);
        *ip_string_e = tmp;
        aes_encrypt(*ip_string_e, AES_DECRYPT);
        tmp = NULL;
    } else
    {
        tmp->len = 0;
        tmp->encrypt_string = NULL;
        *ip_string_e = tmp;
        tmp = NULL;
    }DEBUG_printf("Function end %s\n", "get_backup_ip_string");
}

/**
 * use backup list string to create area_domain_backup struct
 */
void get_backup_struct(area_domain_backup **d_b_list)
{
    DEBUG_printf("Function start %s\n", "get_backup_struct");
    ip_string *ip_string_c = NULL;
    get_backup_ip_string(back_server, &ip_string_c);

    constructor((char *) ip_string_c->source_string, d_b_list);

    free(ip_string_c->encrypt_string);
    free(ip_string_c->source_string);
    free(ip_string_c);
    DEBUG_printf("Function end %s\n", "get_backup_struct");
}

void get_ip_struct()
{
    DEBUG_printf("Function start %s\n", "get_ip_struct");
    ip_string *ip_string_c = NULL;
    get_backup_ip_string(ip_server, &ip_string_c);
    bool is_server_null = true;
    char *next_server = NULL;
    char *server_addr = (char *) ip_string_c->source_string;
    server *server_each_h, *server_each_t;
    server_each_h = server_each_t = NULL;
    if (server_addr != NULL)
    {
        is_server_null = false;
        server_each_h = (server *) calloc(1, sizeof(server));
    }
    while (server_addr != NULL)
    {
        next_server = strchr(server_addr, ',');
        if (next_server != NULL)
            *(next_server++) = '\0';
        server_each_h->address = cp_string(server_each_h->address, server_addr);
        server_each_h->next = NULL;
        if (NULL == server_list)
        {
            server_list = server_each_h;
        } else
        {
            server_each_t->next = server_each_h;
        }
        server_each_t = server_each_h;
        server_each_h = (server *) calloc(1, sizeof(server));
        server_addr = next_server;
    }
    if (!is_server_null)
    {
        free(server_each_h);
    }
    free(ip_string_c->encrypt_string);
    free(ip_string_c->source_string);
    free(ip_string_c);
    DEBUG_printf("Function end %s\n", "get_ip_struct");
}

/**
 * get backup list string an decrypted
 */
char *get_string_from_server()
{
    DEBUG_printf("Function start %s\n", "get_string_from_server");
    char *len = NULL;
    if (NULL == server_list)
    {
        len = get_ip_list(SEVER_IP);
        get_ip_struct();
        DEBUG_printf("1: %s\n", SEVER_IP);
    } else
    {
        server *list = server_list;
        list_for_each(list,server_list)
        {
            len = get_ip_list(list->address);
            DEBUG_printf("2: %s\n", list->address);
            if (len != NULL)
                break;
        }
    }
    if (len == NULL)
    {
        server * free_server_list;
        free_list_server(server_list, free_server_list);
    }
    DEBUG_printf("Function end %s\n", "get_string_from_server");
    return len;

}

bool split_server_string()
{
    DEBUG_printf("Function start %s\n", "split_server_string");
    char *server_string = get_string_from_server();
    if (ip_server != NULL)
    {
        free(ip_server);
    }
    if (server_string != NULL)
    {
        ip_server = server_string;
        back_server = strchr(ip_server, ';');
        if (back_server != NULL)
            *(back_server++) = '\0';
        return true;
    } else
    {

        ip_server = NULL;
        back_server = NULL;
        return false;
    }
}

int get_pool_num(char *msg)
{
    char *h = strstr(msg, INFO_HEAD);
    h += strlen(INFO_HEAD) - 1;
    char *s = h;
    h = strstr(s, ":");
    if (NULL != h)
    {
        *h = '\0';
    }
    return atoi(s);
}

cJSON *exec_api_cmd(char *cmd)
{
    char *pool_json = get_cgminer_api(cmd);
    cJSON *root = cJSON_Parse(pool_json);
    cJSON *status_info = cJSON_GetObjectItem(root, "STATUS");
    bool res = check_api_status(status_info);
    free(pool_json);
    cJSON_Delete(root);
    if (!res)
        return NULL;
    return status_info;
}

void reset_priority()
{
    pools *c_pools = NULL;
    pools *tmp_pools = NULL;
    get_pools_info(&c_pools);
    if (NULL == c_pools)
        return;
    int i = 0;
    list_for_each(tmp_pools,c_pools)
    {
        i++;
    }
    char *set_pri_cmd = NULL;
    set_pri_cmd = (char *) calloc(strlen(SET_PRIORITY_CMD) + i * 2, sizeof(char));
    if (NULL == set_pri_cmd)
        return;

    char *pool_p = (char *) calloc(i * 2, sizeof(char));
    if (NULL == pool_p)
    {
        free(set_pri_cmd);
        return;
    }

    for (for_tag = 0; for_tag < i; for_tag++)
    {
        char buff[2];
        sprintf(buff, "%d,", i);
        strcat(pool_p, buff);
    }
    pool_p[for_tag * 2 - 1] = '\0';
    sprintf(set_pri_cmd, SET_PRIORITY_CMD, pool_p);

    DEBUG_printf("cmd is %s\n", set_pri_cmd);
    exec_api_cmd(set_pri_cmd);

    if (c_pools != NULL)
    {
        pools *free_c_pools;
        free_list_cpools(c_pools, free_c_pools);
    }
    free(pool_p);
    free(set_pri_cmd);
}

void change_priority(int pool, int Priority)
{
    char *set_pri_cmd = NULL;
    set_pri_cmd = (char *) calloc(strlen(SET_PRIORITY_CMD) + pool * 2, sizeof(char));
    if (NULL == set_pri_cmd)
        return;

    char *pool_p = (char *) calloc((pool + 1) * 2, sizeof(char));
    if (NULL == pool_p)
    {
        free(set_pri_cmd);
        return;
    }

    int i = 0;
    for (for_tag = 0; for_tag < pool + 1; for_tag++)
    {
        char buff[2];
        if (for_tag != Priority)
        {
            sprintf(buff, "%d,", i);
            i++;
        } else
        {
            sprintf(buff, "%d,", pool);
        }
        strcat(pool_p, buff);
    }
    pool_p[(pool + 1) * 2 - 1] = '\0';
    sprintf(set_pri_cmd, SET_PRIORITY_CMD, pool_p);

    DEBUG_printf("cmd is %s\n", set_pri_cmd);
    exec_api_cmd(set_pri_cmd);

    free(pool_p);
    free(set_pri_cmd);
}

void check_status_and_backup()
{
    pools *c_pools = NULL;
    area_domain_backup *d_b_list = NULL;
    if (!split_server_string())
    {
        goto END;
    }

    get_pools_info(&c_pools);
    if (NULL == c_pools)
        goto END;

    get_backup_struct(&d_b_list);
    if (NULL == d_b_list)
    {
        goto END;
    }

    pools *this_pool = c_pools;

    if (NULL == ip)
    {
        pool = -1;
    }

    if (pool != -1)
    {
        for (for_tag = 0; for_tag < pool && this_pool->next != NULL; for_tag++)
        {
            this_pool = this_pool->next;
        }
        DEBUG_printf("%d %s", pool, ip);
        DEBUG_printf("%s %s %s\n", this_pool->pool, ip, this_pool->status);
        if ((strstr(this_pool->pool, ip) && (0 == strcmp(this_pool->status, "Dead"))))
        {
            char *remove_cmd = NULL;
            remove_cmd = (char *) calloc(strlen(REMOVE_POOL_CMD) + sizeof(pool), sizeof(char));
            if (NULL == remove_cmd)
                return;

            sprintf(remove_cmd, REMOVE_POOL_CMD, pool);
            DEBUG_printf("cmd is %s\n", remove_cmd);

            exec_api_cmd(remove_cmd);

            free(remove_cmd);
            pool = -1;
            free(ip);
            ip = NULL;
            reset_priority();
            get_pools_info(&c_pools);
            if (NULL == c_pools)
                return;DEBUG_printf("Remove pool%s\n", this_pool->pool);
        } else if (NULL == strstr(this_pool->pool, ip))
        {
            pool = -1;
            free(ip);
            ip = NULL;
        }
    }
    pools *tmp_pools = NULL;
    area_domain_backup *tmp_area = NULL;

    int pool_num = 0;
    list_for_each(tmp_pools,c_pools)
    {
        pool_num++;
    }

    if (pool_num - 1 != pool)
    {
        pool = -1;
        free(ip);
        ip = NULL;
    }

    if (-1 == pool)
    {
        list_for_each(tmp_pools,c_pools)
        {
            list_for_each(tmp_area,d_b_list)
            {
                node *tmp_dl = NULL;
                list_for_each(tmp_dl,tmp_area->domain_list)
                {
                    if ((0 == strcmp(tmp_pools->status, "Dead")) && strstr(tmp_pools->pool, tmp_dl->ip)
                            && !tmp_dl->isreachable)
                    {
                        node *tmp_bl = NULL;
                        list_for_each(tmp_bl,tmp_area->backup_list)
                        {

                            DEBUG_printf("backup pool :%s bool:%d\n", tmp_bl->ip, tmp_bl->isreachable);
                            if (tmp_bl->isreachable)
                            {
                                char *add_cmd = NULL;
                                add_cmd = (char *) calloc(
                                        strlen(ADD_POOL_CMD) + strlen(tmp_bl->ip) + strlen(tmp_pools->user)
                                                + strlen("x") + 1, sizeof(char));
                                if (NULL == add_cmd)
                                    return;
                                sprintf(add_cmd, ADD_POOL_CMD, tmp_bl->ip, tmp_bl->port, tmp_pools->user, "x");
                                DEBUG_printf("cmd is %s\n", add_cmd);
                                cJSON *tmp = exec_api_cmd(add_cmd);
                                free(add_cmd);
                                cJSON *s = NULL;
                                if (tmp != NULL)
                                {
                                    s = cJSON_GetArrayItem(tmp, 0);
                                }
                                if (s != NULL)
                                {
                                    char *ss = cJSON_GetObjectItem(s, "Msg")->valuestring;
                                    pool = get_pool_num(ss);
                                    ip = (char *) calloc(1, strlen(tmp_bl->ip) + 1);
                                    strcpy(ip, tmp_bl->ip);
                                    change_priority(pool, tmp_pools->Priority);
                                    goto END;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    END: ;
    if (d_b_list != NULL)
    {
        DEBUG_printf("Free dblist");
        area_domain_backup *free_d_b_list;
        node *free_domain_list;
        node *free_backup_list;
        list_for_each(free_d_b_list,d_b_list)
        {
            free_list_node(free_d_b_list->backup_list, free_backup_list);
            free_list_node(free_d_b_list->domain_list, free_domain_list);
        }

        free_list(d_b_list, free_d_b_list);
    }
    if (c_pools != NULL)
    {
        DEBUG_printf("Free c_pools");
        pools *free_c_pools;
        free_list_cpools(c_pools, free_c_pools);
    }
}

void set_time(int interval_for_check_ddos)
{
    struct itimerval itv;
    itv.it_interval.tv_sec = interval_for_check_ddos;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = interval_for_check_ddos;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, NULL);
}

void signal_check()
{
    set_time(INTERVAL_FOR_CHECK_DDOS);
    signal(SIGALRM, check_status_and_backup);
}

int main(int argc, char **arg)
{
    DEBUG_printf("d-ddos start!");
    while (13)
    {
        check_status_and_backup();
        sleep(INTERVAL_FOR_CHECK_DDOS);
    }
    return 0;
}
