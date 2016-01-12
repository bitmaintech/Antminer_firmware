#ifndef _CLIENT_H
#define _CLIENT_H
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
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/time.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
 #include <openssl/engine.h>
#include <openssl/conf.h>
#include "config.h"
#include<sys/param.h>
#include<sys/stat.h>

#define BUFFER_SIZE 4096
#define COMMAND_SUM 9

#define MAXQSIZE 9       // queue size MAXQSIZE-1
#define NO_DATA_TIME 26 //How many seconds to detect any data 26*30S=780S about 15min
#define WIAT_RES_TIMEOUT 30  //60s//10min

#define BB_BLACK_CONF_SWITCH "api-switch"
#define OPENWRT_CONF_SWITCH "off_on"
//#define MINERTYPE
char *ip_toconnect="192.168.1.187";

int time_count = 0;

extern int sockfd;
extern SSL *ssl;
extern SSL_CTX *ctx;
extern int sendbytes;
extern int recvbytes; //establish a socket with the server
extern char buf[BUFFER_SIZE];//Buf to the server
extern char serv_v[3];       //Receive the server start two bytes
extern char serv_cmd[3];     //Server command
extern char body_len[2];     //The length of the server package
extern char auth_id[70];     //save cUNIQID

extern int pthr_rev_id;

extern char *conf_directory;  //The configuration file directory
extern char *luci_conf_directory; //Web profile directory
extern char *luci_cgminer_dir;
extern char *bb_black_cgc_dir;
extern char *bb_black_conf_dir;


#ifdef OPENWRT
char *luci_conf_directory= "/etc/config/monitor-webset"; //the configuration file directory will be read
char *luci_cgminer_dir="/etc/config/cgminer";
#endif
#ifdef BB_BLACK
char *bb_black_conf_dir="/config/minermonitor.conf";
char *bb_black_cgc_dir = "/config/cgminer.conf";
#endif

#define DATAFORMAT_CONF_PAHT "/config/dataformatconfig/"
char *server_command_code[COMMAND_SUM]={"01","02","03","04","05","06","07","08","09"};//source command code from server
char *command_code[COMMAND_SUM]={"10","20","30","40","50","60","70","80","90"};//after switch/reply
char *server_command[COMMAND_SUM]={"AUTH","devs","summary","stats","pools","","lcd","",""};
char *dataformat_conf_dir[COMMAND_SUM]={"",DATAFORMAT_CONF_PAHT"dataformat_devs.conf",DATAFORMAT_CONF_PAHT"dataformat_summary.conf",DATAFORMAT_CONF_PAHT"dataformat_stats.conf",DATAFORMAT_CONF_PAHT"dataformat_pools.conf","",DATAFORMAT_CONF_PAHT"dataformat_lcds.conf","",""};

char *pid_lock_dir = "/tmp/miner-monitor/pid";//Locking the file or not to realize singleton

#endif //_CLIENT_H
















