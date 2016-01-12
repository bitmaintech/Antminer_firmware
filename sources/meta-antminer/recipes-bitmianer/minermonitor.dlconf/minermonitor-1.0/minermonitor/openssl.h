#ifndef _OPENSSL_H
#define _OPENSSL_H

#include <string.h>
#include <openssl/hmac.h>
#include "read_conf.h"
#include "gen_middata.h"
char nonce[10];
char *api_key_send(char *data);
char *nonce_send(char *data_nance);
char *api_key_ssl(char *data,char *result_hex);



#endif
