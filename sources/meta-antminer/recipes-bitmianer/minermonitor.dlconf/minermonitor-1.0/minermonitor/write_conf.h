#ifndef _WRITE_CONF_H
#define _WRITE_CONF_H
#include <stdio.h>
#include "cjson.h"

#include "debug.h"
#include "send_pkg.h"
extern char rev_ID[40];

typedef struct //The items to write cgminer configuration file
{
    char which_pool[4];
    char url[80];
    char user[40];
    char pass[40];
}POOLS_Data;

typedef struct
{
    POOLS_Data pools_data[3];
    int pools_count;
}POOLS_D_COUNT;

typedef struct
{
    POOLS_Data pools_data[3];
    int api_listen;
    int api_network;
    int bitmain_nobeeper;
    int bitmain_notempoverctrl;
    char api_allow[10];
    char bitmain_freq[15];
    char bitmain_voltage[10];
}GETS4_CONFIG;

typedef struct
{
    POOLS_Data pools_data[3];
    char api_allow[20];
    char more_options[10];
    char freq [15];
    char voltage [10];
    char pool_balance[10];
    char bitmain_nobeeper[10];
    char bitmain_notempoverctrl[10];
}GETS3_CONFIG;

POOLS_D_COUNT *rev_pools(char *,POOLS_D_COUNT *get_pool_data);
GETS4_CONFIG *save_old_conf(char* fileName,GETS4_CONFIG *get_save_conf);
GETS4_CONFIG *creat_newpools(POOLS_D_COUNT *change,GETS4_CONFIG *fresh_old);
void create_conf(GETS4_CONFIG *new_pools,char *dir);
char *read_bb_black_switch(char* fileName,char *key,char *value);
char *read_conf_by_type(char* fileName,char *key,char *value);

GETS3_CONFIG *save_openwrt_webconf(GETS3_CONFIG *save_config);
GETS3_CONFIG *creat_openwrt_newpools(POOLS_D_COUNT *change,GETS3_CONFIG *refresh);
void write_openwrt_conf(GETS3_CONFIG *newdata);
int write_bb_black_respond(char* fileName,char *rev);
int write_openwrt_auth(char *res);
int turnoff_bb_black(char* fileName);
#endif
