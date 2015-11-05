/*
 * get_ip.h
 *
 *  Created on: Mar 17, 2015
 *      Author: fazio
 */

#ifndef INCLUDE_GET_IP_H_
#define INCLUDE_GET_IP_H_

#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

size_t handle_res(void *buffer, size_t size, size_t nmem, void *userp);
char * get_ip_list (char *url);
unsigned char *hex_to_uc (char *hex_string);


#endif /* INCLUDE_GET_IP_H_ */
