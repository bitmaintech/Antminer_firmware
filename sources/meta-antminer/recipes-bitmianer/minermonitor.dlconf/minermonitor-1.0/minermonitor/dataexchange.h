#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
int r_connect;
int (*receivedata)();
int (*senddata)();

int encryptreceivedata(SSL *ssl,int sockfd,char *buf,int len);
int encryptsenddata(SSL *ssl,int sockfd,char *buf,int len);

int sockreceivedata(SSL *ssl,int sockfd,char *buf,int len);
int socksenddata(SSL *ssl,int sockfd,char *buf,int len);
