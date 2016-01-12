#include "dataexchange.h"
#include <openssl/ssl.h>
#include <sys/socket.h>
#include "openssl.h"


int (*receivedata)();
int (*senddata)();
r_connect = 1;

int encryptreceivedata(SSL *ssl,int sockfd,char *buf,int len)
{
    return r_connect == 1 ? SSL_read(ssl,buf,len) : -1;
}
int encryptsenddata(SSL *ssl,int sockfd,char *buf,int len)
{
    return r_connect == 1 ? SSL_write(ssl,buf,len) : -1;
}

int sockreceivedata(SSL *ssl,int sockfd,char *buf,int len)
{
    return r_connect == 1 ? recv(sockfd,buf,len,0) : -1;
}
int socksenddata(SSL *ssl,int sockfd,char *buf,int len)
{
    return r_connect == 1 ? send(sockfd,buf,len,0) : -1;
}



