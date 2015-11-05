/*
 * ping.h
 *
 *  Created on: Mar 20, 2015
 *      Author: fazio
 */

#ifndef SRC_PING_H_
#define SRC_PING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include "debug.h"

#define SOCKETFAIL(a) ((a) < 0)
#define INVSOCK -1
#define CLOSESOCKET close
#define SOCKETINIT {}
#define SOCKERRMSG strerror(errno)

#define SOCKSIZ 65535

bool isreachable_timeout(char *m_server, int m_port, int time_out);

#endif /* SRC_PING_H_ */
