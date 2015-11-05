#include "ping.h"

bool isreachable_timeout(char *m_server, int m_port, int time_out)
{

    struct hostent* host = NULL;
    struct sockaddr_in saddr;
    unsigned int s = 0;
    bool ret;
    int error;
    host = gethostbyname(m_server);
    if (host == NULL)
        ret = false;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(m_port);
    saddr.sin_addr = *((struct in_addr*) host->h_addr);

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ret = false;
    }

    fcntl(s, F_SETFL, O_NONBLOCK);

    if (connect(s, (struct sockaddr*) &saddr, sizeof(saddr)) == -1)
    {
        if (errno == EINPROGRESS)
        { // it is in the connect process
            struct timeval tv;
            fd_set writefds;
            tv.tv_sec = time_out;
            tv.tv_usec = 0;
            FD_ZERO(&writefds);
            FD_SET(s, &writefds);
            if (select(s + 1, NULL, &writefds, NULL, &tv) > 0)
            {
                unsigned int len = sizeof(unsigned int);
                getsockopt(s, SOL_SOCKET, SO_ERROR, &error, &len);
                if (error == 0)
                    ret = true;
                else
                    ret = false;
            } else
                ret = false; //timeout or error happen
        } else
            ret = false;
    } else
        ret = false;

    CLOSESOCKET(s);
    return ret;
}
