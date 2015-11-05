/*
 * aes.h
 *
 *  Created on: Mar 17, 2015
 *      Author: fazio
 */

#ifndef SRC_AES_H_
#define SRC_AES_H_

#include <openssl/aes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct ip_string
{
    int len;
    unsigned char *source_string;
    unsigned char *encrypt_string;
} ip_string;

bool aes_encrypt(ip_string *ip_c, int e_or_d);

#endif /* SRC_AES_H_ */
