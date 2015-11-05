#ifndef _CGMINER_API_H
#define _CGMINER_API_H
#include <string.h>
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
#include "read_conf.h"

#define CG_RECVSIZE 65500             //读gminer-api的buf
#define ITEM_NUM 40
#define ITEM_STRLEN 40
#define COMMAND_SUM 7

extern char buf_deal[CG_RECVSIZE];
static const char SEPARATOR = '|';
static const char COMMA = ',';
static const char EQ = '=';


//


char *callapi(char *api_command, char *host, short int port);
char *data_format(char *api_command,char *buf_data,char *buf_tmp);
//
#endif
