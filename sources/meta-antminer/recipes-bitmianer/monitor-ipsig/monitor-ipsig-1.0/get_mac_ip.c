#include "get_mac_ip.h"
#include "debug.h"

//////////////////求得cUNIQID的值/////////////////////
//////////////////获取6位数字/////////////////////////
//////////////////不够6位的要补齐6位，方式为在前面加0/////
char *gen_cuniqid(char *uniqid )
{
 char str[7];
 char temp_len;
 struct timeval the_time;
 gettimeofday(&the_time,0);
 //DEBUG_printf("%ld\n",the_time.tv_usec);
 sprintf(str,"%ld",the_time.tv_usec);
 memset(uniqid,0,10);
 int size=strlen(str);
 //DEBUG_printf("size=%d\n",size);
 if((temp_len=6-size)>0)
  memset(uniqid,'0',temp_len);//不够6位的要补齐6位，方式为在前面加0
 strcat(uniqid,str);
 return uniqid;
}


//////////////////////////////////////////////////
////////////得到IP地址///////////////
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
  printf(":No Such Device %s/n",eth);
  return -1;
 }

 memcpy(&my_addr, &ifr.ifr_addr, sizeof(my_addr));
 strcpy(ipaddr, inet_ntoa(my_addr.sin_addr));
 close(sock_fd);
 return 1;
}

//////////////////////////////////////////////////
////////////得到网卡MAC地址＊///////////////
char *get_mac(char *mac,char *ethn)
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
    strcpy (ifreq.ifr_name, ethn);    //只得到eth0的地址
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

char *get_macforkey(char *mac,char *ethn)
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
    strcpy (ifreq.ifr_name, ethn);    //只得到eth0的地址
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
  
    strcpy(mac,mac_tmp);
    free(mac_tmp);
    return mac;
}


