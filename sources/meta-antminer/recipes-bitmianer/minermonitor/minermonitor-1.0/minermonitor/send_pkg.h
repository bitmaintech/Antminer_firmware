#ifndef _SEND_PKG_H
#define _SEND_PKG_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <curl/curl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/time.h>
#include <openssl/hmac.h>
#include "read_conf.h"
#include "openssl.h"
#include "gen_middata.h"
#include "cgminer_api.h"
#include "write_conf.h"

#define DEVS     "devs"
#define SUMMARY  "summary"
#define POOLS    "pools"
#define STATS    "stats"
#define PKG_NUM  6
#define BUFFER_SIZE 4096
char *s4_conf_dir;
char *s4_cgc_dir;
typedef struct PKG_Format
{
  char pkg_head[3];
  char pkg_cmd[3];
  unsigned char pkg_body_len[2];
  char pkg_body[4096];
  char pkg_nonce[7];
} PKG_Format;

extern PKG_Format package;
extern PKG_Format deal_package;

extern char *command_code[COMMAND_SUM];
extern char *server_command_code[COMMAND_SUM];
extern char *server_command[COMMAND_SUM];
extern char *dataformat_conf_dir[COMMAND_SUM];


int  rev_data();
int  rev_data_struct();
void rev_respond();
char send_head(char *cmd,unsigned int body_len);
void send_connect_auth(char *cmd);
int send_connect_cc(char *cmd);
int send_getstatus_res(char *cmd_server);
char restart_cgminer();
void write_config1();
void send_setconfig_result(char *cmd,char *conf_stat);
int set_config(char *cmd);
void curl_get();
int updata() ;
void clear_recv(PKG_Format pkg_struct);
int deal_cmd(PKG_Format pkg_struct);
#endif
