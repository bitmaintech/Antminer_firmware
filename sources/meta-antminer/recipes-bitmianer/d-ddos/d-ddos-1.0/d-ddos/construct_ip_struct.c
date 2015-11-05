#include "construct_ip_struct.h"

static const char AREA_SEP = '|';
static const char TYPE_SEP = ';';
static const char ITEM_SEP = ',';
static const char PORT_SEP = ':';

char * cp_string(char * to,char *from)
{
    to = (char *)calloc(strlen(from)+1,sizeof(char));
    strcpy(to,from);
    return to;
};

void constructor(char *buf, area_domain_backup **backup_list)
{
    char *next_area, *type, *next_type, *item, *next_item, *port;
    node *back_node_h, *back_node_t, *domain_node_h, *domain_node_t;

    area_domain_backup *area_h, *area_t;

    back_node_h = back_node_t = NULL;
    domain_node_h = domain_node_t = NULL;
    area_h = area_t = NULL;
    back_node_h = (node *) calloc(1, sizeof(node));
    domain_node_h = (node *) calloc(1, sizeof(node));
    area_h = (area_domain_backup *) calloc(1, sizeof(area_domain_backup));

    area_h->domain_list = NULL;
    area_h->backup_list = NULL;
    area_h->domain_num = 0;
    area_h->backup_num = 0;
    area_h->next=NULL;

    int type_count;
    while (buf != NULL)
    {
        next_area = strchr(buf, AREA_SEP);
        if (next_area != NULL)
            *(next_area++) = '\0';
        if (*buf && *buf != '\n')
        {
            DEBUG_printf("area: %s\n", buf);
            type = buf;
            type_count = 0;
            while (type != NULL)
            {
                next_type = strchr(type, TYPE_SEP);
                if (next_type != NULL)
                    *(next_type++) = '\0';
                if (*type)
                {
                    DEBUG_printf("type: %s\n", type);
                    item = type;
                    while (item != NULL)
                    {
                        next_item = strchr(item, ITEM_SEP);
                        if (next_item != NULL)
                            *(next_item++) = '\0';
                        if (0 == type_count)
                        {
                            port = strchr(item, PORT_SEP);
                            if (port != NULL)
                                *(port++) = '\0';
                            domain_node_h->ip = cp_string(domain_node_h->ip,item);
                            domain_node_h->port = cp_string(domain_node_h->port,port);
                            domain_node_h->isreachable = isreachable_timeout(item, atoi(port), 10);
                            domain_node_h->next = NULL;
                            if (NULL == area_h->domain_list)
                            {
                                area_h->domain_list = domain_node_h;
                            } else
                            {
                                domain_node_t->next = domain_node_h;
                            }
                            area_h->domain_num++;
                            domain_node_t = domain_node_h;
                            domain_node_h = (node *) malloc(sizeof(node));
                            DEBUG_printf("domain: %s\n", item);
                        } else
                        {
                            port = strchr(item, PORT_SEP);
                            if (port != NULL)
                                *(port++) = '\0';
                            back_node_h->ip = cp_string(back_node_h->ip,item);
                            back_node_h->port = cp_string(back_node_h->port,port);
                            back_node_h->isreachable = isreachable_timeout(item, atoi(port), 10);
                            back_node_h->next = NULL;
                            if (NULL == area_h->backup_list)
                            {
                                area_h->backup_list = back_node_h;
                            } else
                            {
                                back_node_t->next = back_node_h;
                            }
                            area_h->backup_num++;
                            back_node_t = back_node_h;
                            back_node_h = (node *) malloc(sizeof(node));
                            DEBUG_printf("backup: %s\n", item);
                        }
                        item = next_item;
                    }
                }
                type = next_type;
                type_count++;
            }
            if (NULL == *backup_list)
            {
                *backup_list = area_h;
            } else
            {
                area_t->next = area_h;
            }
            area_t = area_h;
            area_h = (area_domain_backup *) calloc(1, sizeof(area_domain_backup));

            area_h->domain_list = NULL;
            area_h->backup_list = NULL;
            area_h->domain_num = 0;
            area_h->backup_num = 0;
            area_h->next = NULL;

        }
        buf = next_area;
    }
    free(back_node_h);
    free(domain_node_h);
    free(area_h);
}
