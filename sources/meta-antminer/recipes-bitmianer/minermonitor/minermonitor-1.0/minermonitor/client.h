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
//#include "miner.h"
//#include "config.h"
//#include "compat.h"
//#include <jansson.h>
//#include <curl/curl.h>
//////////////////////////////////////////////////////
////////////Data structure variables////////////////////////////////
//#define CG_RECVSIZE 65500             //Read the buf of gminer-api
#define SERVER_IP     "192.168.1.185" //Connect to the server address
#define SERVER_PORT   2000            //the port of server
#define CONNECT_MAX 5
#define BUFFER_SIZE 4096
#define PKG_NUM  6
#define COMMAND_SUM 7


#define MAXQSIZE 9       // queue size MAXQSIZE-1
#define NO_DATA_TIME 26 //How many seconds to detect any data 26*30S=780S about 15min
#define WIAT_RES_TIMEOUT 30  //60s//10min
#define BB_BLACK_CONF_SWITCH "api-switch"
#define OPENWRT_CONF_SWITCH "off_on"
//#define MINERTYPE
char *ip_toconnect="192.168.1.187";

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
extern char *s4_cgc_dir;
extern char *s4_conf_dir;


#ifdef OPENWRT
char *luci_conf_directory= "/etc/config/monitor-webset"; //the configuration file directory will be read
char *luci_cgminer_dir="/etc/config/cgminer";
#endif
#ifdef BB_BLACK
char *s4_conf_dir="/config/minermonitor.conf";
char *s4_cgc_dir = "/config/cgminer.conf";
#endif

char *command_code[COMMAND_SUM]={"10","20","30","40","50","60","70"};//after switch
char *server_command_code[COMMAND_SUM]={"01","02","03","04","05","06","07"};//source command code from server
char *server_command[COMMAND_SUM]={"AUTH","devs","summary","stats","pools","","lcd"};
char *dataformat_conf_dir[COMMAND_SUM]={"","/config/dataformatconfig/dataformat_devs.conf","/config/dataformatconfig/dataformat_summary.conf","/config/dataformatconfig/dataformat_stats.conf","/config/dataformatconfig/dataformat_pools.conf","","/config/dataformatconfig/dataformat_lcds.conf"};

char *pid_lock_dir = "/tmp/miner-monitor/pid";

#ifdef PC_IP
//char *s4_conf_dir="minermonitor.conf";
//char *s4_cgc_dir = "cgminer.conf";
//char *conf_directory= "monitor.conf"; //the configuration file directory will be read
char *luci_conf_directory= "/home/sonny/Desktop/s3_ssl/S3_ssl/bin/Debug/monitor-webset"; //the luci configuration file directory will be read
char *luci_cgminer_dir="cgminer";
#endif

#endif
















