#include "gen_middata.h"
#include "debug.h"

//////////////////get cUNIQID/////////////////////
//////////////////get 6 digits/////////////////////////
//////////////////not enough 6 up to 6,eay to add 0 in front/////
char *gen_cuniqid(char *uniqid )
{
    char str[7];
    char temp_len;
    struct timeval the_time;
    gettimeofday(&the_time,0);
    sprintf(str,"%ld",the_time.tv_usec);
    memset(uniqid,0,7);
    int size=strlen(str);
    if((temp_len=6-size)>0)
    memset(uniqid,'0',temp_len);//not enough 6 up to 6,eay to add 0 in front
    strcat(uniqid,str);
    return uniqid;
}
//////////////////////////////////////////////////
////////////get IP address///////////////
int get_ip(char *eth, char *ipaddr)
{
    int sock_fd;
    struct  sockaddr_in my_addr;
    struct ifreq ifr;

 /**//* Get socket file descriptor */
    if ((sock_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
    perror("socket");
    return -1;
    }
 /**//* Get IP Address */
    strncpy(ifr.ifr_name, eth,20 );
    ifr.ifr_name[IFNAMSIZ-1]='0';

    if (ioctl(sock_fd, SIOCGIFADDR, &ifr) < 0)
    {
        DEBUG_printf(":No Such Device %s/n",eth);
        return -1;
    }

    memcpy(&my_addr, &ifr.ifr_addr, sizeof(my_addr));
    strcpy(ipaddr, inet_ntoa(my_addr.sin_addr));
    close(sock_fd);
    return 1;
}

//////////////////////////////////////////////////
//////////// default eth0 when get the mac address///////////////
char *get_mac(char *mac,char *ethx)
{
    struct ifreq ifreq;
    int sock;
    char *mac_tmp;
    mac_tmp=(char*)calloc(40,sizeof(char));

    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror ("socket");
       exit(0);
    }
    strcpy (ifreq.ifr_name, ethx);    //only get the address of eth0
    if (ioctl (sock, SIOCGIFHWADDR, &ifreq) < 0)
    {
        perror ("ioctl");
        exit(0);
    }
    close(sock);
     sprintf (mac_tmp, "%02X:%02X:%02X:%02X:%02X:%02X",
             (unsigned char) ifreq.ifr_hwaddr.sa_data[0], (unsigned char) ifreq.ifr_hwaddr.sa_data[1],
             (unsigned char) ifreq.ifr_hwaddr.sa_data[2], (unsigned char) ifreq.ifr_hwaddr.sa_data[3],
             (unsigned char) ifreq.ifr_hwaddr.sa_data[4], (unsigned char) ifreq.ifr_hwaddr.sa_data[5]);
    strcpy(mac,"MAC=");
    strcat(mac,mac_tmp);
    strcat(mac,",");
    free(mac_tmp);
    return mac;

}




