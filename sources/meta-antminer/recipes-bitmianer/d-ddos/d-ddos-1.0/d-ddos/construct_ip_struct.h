/*
 * construct_ip_struct.h
 *
 *  Created on: Mar 19, 2015
 *      Author: fazio
 */

#ifndef SRC_CONSTRUCT_IP_STRUCT_H_
#define SRC_CONSTRUCT_IP_STRUCT_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "ping.h"

typedef struct node
{
    char *ip;
    char *port;
    bool isreachable;
    struct node *next;
} node;

typedef struct area_domain_backup
{
    struct node *domain_list;
    int domain_num;
    struct node *backup_list;
    int backup_num;
    struct area_domain_backup *next;
} area_domain_backup;

void constructor(char *, area_domain_backup **);
char * cp_string(char * to,char *from);

#endif /* SRC_CONSTRUCT_IP_STRUCT_H_ */
