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

#include "cgminer_api.h"
#include "debug.h"

#define SOCKETFAIL(a) ((a) < 0)
#define INVSOCK -1
#define CLOSESOCKET close
#define SOCKETINIT {}
#define SOCKERRMSG strerror(errno)

#define SOCKSIZ 65535

char *callapi(char *command, char *host, short int port)
{
    struct hostent *ip;
    struct sockaddr_in serv;
    int sock;
    int n;
    char *buf = NULL;
    size_t len, p;

    SOCKETINIT;

    ip = gethostbyname(host);
    if (!ip)
    {
        DEBUG_printf("Couldn't get hostname: '%s'\n", host);
        return NULL;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVSOCK)
    {
        DEBUG_printf("Socket initialisation failed: %s\n", SOCKERRMSG);
        return NULL;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr = *((struct in_addr *) ip->h_addr);
    serv.sin_port = htons(port);

    if (SOCKETFAIL(connect(sock, (struct sockaddr * )&serv, sizeof(struct sockaddr))))
    {
        DEBUG_printf("Socket connect failed: %s\n", SOCKERRMSG);
        goto END;
    }

    n = send(sock, command, strlen(command), 0);
    if (SOCKETFAIL(n))
    {
        DEBUG_printf("Send failed: %s\n", SOCKERRMSG);
    } else
    {
        len = SOCKSIZ;
        buf = (char *) malloc(len + 1);
        if (!buf)
        {
            DEBUG_printf("Err: OOM (%d)\n", (int ) (len + 1));
            goto END;
        }
        p = 0;
        while (42)
        {
            if ((len - p) < 1)
            {
                len += SOCKSIZ;
                buf = (char *) realloc(buf, len + 1);
                if (!buf)
                {
                    DEBUG_printf("Err: OOM (%d)\n", (int ) (len + 1));
                    goto END;
                }
            }

            n = recv(sock, &buf[p], len - p, 0);

            if (SOCKETFAIL(n))
            {
                DEBUG_printf("Recv failed: %s\n", SOCKERRMSG);
                break;
            }

            if (n == 0)
                break;

            p += n;
        }
        buf[p] = '\0';
    }

    END: CLOSESOCKET(sock);
    return buf;
}
