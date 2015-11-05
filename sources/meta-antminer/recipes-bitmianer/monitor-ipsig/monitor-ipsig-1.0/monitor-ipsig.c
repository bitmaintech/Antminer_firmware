#include <linux/input.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "get_mac_ip.h"
#include "debug.h"
#define DELAY_2S   sleep(2)
#define FOR_2MIM     5
#define KEY_TIMES   5
typedef enum{false=0,true=!false} bool ;

int udp_sock = -1;
struct sockaddr_in addrto;
struct sockaddr_in from;
int key_value=0;
char mac_tmp[30] = {0};
int interval_count=0;

//GPIO 2_2
//GPIO2_5 recover_key GPIO2_3
//Rev 001
/*
# gpio0_23 = 23 = green LED
# gpio0_26 = 26  = Key
# gpio0_46 = 46  = Key2
# gpio1_13 = 45 = red LED
# gpio0_26 = 20 = beep
*/
void rg_off(void)
{
	system("echo low >/sys/class/gpio/gpio23/direction");
	system("echo low >/sys/class/gpio/gpio45/direction");
}

void rg_on(void)
{
	system("echo high >/sys/class/gpio/gpio23/direction");
	system("echo high >/sys/class/gpio/gpio45/direction");
}

//Key GPIO number 46
bool getkey_gpio_46()
{
   unsigned int value;
   FILE *pRreco = NULL;
   if(!isExist("/sys/class/gpio/gpio46"))
   {
	unsigned int value;
	FILE *pRreco = NULL;
	if((pRreco = fopen("/sys/class/gpio/export", "w")) == NULL)
	{
	    printf("Open read gpio/export\n");
		return -1;
	}
   if(fwrite("46", strlen("46"), 1, pRreco) != 1)
    printf("File Write Error!\n");
	fclose(pRreco);
  }
	if((pRreco = fopen("/sys/class/gpio/gpio46/value","r")) == NULL)
	{
	    printf("Open read recovery button failure\n");
		return -1;
	}
	fscanf(pRreco,"%d",&value);
	fclose(pRreco);
	if(value)
		return true;
	else
		return false;
}
//Key GPIO number 26
bool getkey_gpio_26()
{
   unsigned int value;
   FILE *pRreco = NULL;
   if(!isExist("/sys/class/gpio/gpio26"))
   {
	unsigned int value;
	FILE *pRreco = NULL;
	if((pRreco = fopen("/sys/class/gpio/export", "w")) == NULL)
	{
	    printf("Open read gpio/export\n");
		return -1;
	}
   if(fwrite("26", strlen("26"), 1, pRreco) != 1)
    printf("File Write Error!\n");
	fclose(pRreco);
  }
	if((pRreco = fopen("/sys/class/gpio/gpio26/value","r")) == NULL)
	{
	    printf("Open read recovery button failure\n");
		return -1;
	}
	fscanf(pRreco,"%d",&value);
	fclose(pRreco);
	if(value)
		return true;
	else
		return false;
}
//To determine whether a file exists
int isExist(char *filename)
{
 return (access(filename, 0) == 0);
}
//Green light quickly Blink a few times, a buzzer rang a time
int success_action()
{
  if(!isExist("/sys/class/gpio/gpio20"))
  {
	unsigned int value;
	FILE *pRreco = NULL;
	if((pRreco = fopen("/sys/class/gpio/export", "w")) == NULL)
	{
	    printf("Open read gpio/export\n");
		return -1;
	}
   if(fwrite("20", strlen("20"), 1, pRreco) != 1)
    printf("File Write Error!\n");
	fclose(pRreco);
  }
  if(!isExist("/sys/class/gpio/gpio45"))
  {
	unsigned int value;
	FILE *pRreco = NULL;
	if((pRreco = fopen("/sys/class/gpio/export", "w")) == NULL)
	{
	    printf("Open read gpio/export\n");
		return -1;
	}
   if(fwrite("45", strlen("45"), 1, pRreco) != 1)
    printf("File Write Error!\n");
	fclose(pRreco);
  }

  int i;
  system("echo low > /sys/class/gpio/gpio20/direction");
  system("echo 1 > /sys/class/gpio/gpio20/value");
  usleep (200*1000);
  system("echo 0 > /sys/class/gpio/gpio20/value");

  for(i=0;i<3;i++)
  {
    system("echo high >/sys/class/gpio/gpio23/direction");
    usleep (50*1000);
    system("echo low >/sys/class/gpio/gpio23/direction");
    usleep (50*1000);
  }
}
//The Red light Blinking fast,continue 6s Red Light && buzzer
int failure_action()
{ int i;
  if(!isExist("/sys/class/gpio/gpio20")) //To determine whether a file exists，nor,create
  {
	unsigned int value;
	FILE *pRreco = NULL;
	if((pRreco = fopen("/sys/class/gpio/export", "w")) == NULL)
	{
	    printf("Open read gpio/export\n");
		return -1;
	}
   if(fwrite("20", strlen("20"), 1, pRreco) != 1)
    printf("File Write Error!\n");
	fclose(pRreco);
  }
  if(!isExist("/sys/class/gpio/gpio23"))
  {
	unsigned int value;
	FILE *pRreco = NULL;
	if((pRreco = fopen("/sys/class/gpio/export", "w")) == NULL)
	{
	    printf("Open read gpio/export\n");
		return -1;
	}
   if(fwrite("23", strlen("23"), 1, pRreco) != 1)
    printf("File Write Error!\n");
	fclose(pRreco);
  }

  for(i=0;i<3;i++)
  {
    system("echo low > /sys/class/gpio/gpio20/direction");
    system("echo 1 > /sys/class/gpio/gpio20/value");
    system("echo low >/sys/class/gpio/gpio45/direction");
    usleep (50*1000);
    system("echo high >/sys/class/gpio/gpio45/direction");
    system("echo 0 > /sys/class/gpio/gpio20/value");
    usleep (50*1000);
  }
}

int udp_init()
{
  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("socket error");
		return -1;
	}

	const int opt = 1;
	//set socker broadcast type
	int nb = 0;
	nb = setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
	 printf("set socket error...");
	 return -1;
	}
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family=AF_INET;
	addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	addrto.sin_port=htons(14235);

    ///*
	bzero(&from, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	from.sin_port = htons(14236);
	if(bind(udp_sock,(struct sockaddr *)&(from), sizeof(struct sockaddr_in)) == -1)
	{
		printf("bind error...");
		return 0;
	}
   //*/
}

int send_ipmac()              //sleep 2s ,发送一次，接收函数需要无阻塞
{
  char sendmsg[100];
  char mac_data[30];
  char ipaddr[40];
  memset(mac_data,0,sizeof(char)*30);
  memset(ipaddr,0,sizeof(char)*40);
  memset(sendmsg,0,sizeof(char)*100);

  if(!get_ip("eth0",ipaddr))
  {
     DEBUG_printf("get IP error\n");
  }
  get_macforkey(mac_data,"eth0");
  strncpy(mac_tmp,mac_data,strlen(mac_data));
  strcpy(sendmsg,ipaddr);
  strcat(sendmsg,",");
  strcat(sendmsg,mac_data);

  int ret=sendto(udp_sock, sendmsg, strlen(sendmsg), 0, (struct sockaddr*)&addrto, sizeof(addrto));
  if(ret<0)
  {
   DEBUG_printf("send error....");
  }
  else
  {
   DEBUG_printf("send ipmac:\n%s\n",sendmsg);
  }
 //DELAY_2S;
  return 1;
 }

int send_ack(char *ack)
{
  int ret=sendto(udp_sock, ack, (size_t)10, 0, (struct sockaddr*)&addrto, sizeof(addrto));
  if(ret<0)
  {
   DEBUG_printf("send error....");
  }
  else
  {
   DEBUG_printf("send ack OK!\n ");
  }
}

//if retrun 1,then break the check_result
//if return 0,then Continue unfinished sent 2 minutes
int check_result()
{
  fd_set rev_fd;
  struct timeval timeout={2,0};
  FD_ZERO(&rev_fd);
  FD_SET(udp_sock,&rev_fd); //Add the descriptor
 char *r_mac=NULL;
 r_mac=(char*)calloc(30,sizeof(char));

 switch(select((udp_sock+1),&rev_fd,NULL,NULL,&timeout))
  {
   case -1: exit(-1);
   break;
   case 0:interval_count++;
   break;
   default:
   if(FD_ISSET(udp_sock,&rev_fd)) //read test
   {
    //从广播地址接受消息
    int nlen=sizeof(from);
    int ret=recvfrom(udp_sock, r_mac, (size_t)30, 0, (struct sockaddr*)&from,(socklen_t*)&nlen);
    if(ret<=0)
    {
     DEBUG_printf("read error....");
     interval_count++;
    }
  else
  {
   DEBUG_printf("rev:\n%s\t", r_mac);
   interval_count=0;
   if(0==strncmp(r_mac,mac_tmp,strlen(mac_tmp))) // The match is successful
   {
     //匹配发送1  &&  绿灯快速几次，蜂鸣器响一声
     DEBUG_printf("send_ack(\"1\");");
     success_action();
     send_ack("OK");
   }
  else
   {                                            //Matching is not successful
     //不匹配发送0 && 红灯快速闪烁，蜂鸣器响1s以上
      DEBUG_printf("send_ack(\"0\");");
     failure_action();
     send_ack("FAILD");
     free(r_mac);
     return 0;  //not Matching ,Continue unfinished sent 2 minutes
   }
   free(r_mac);
   return 1;  //OK OK
  }
  }break;
 }
 if(interval_count==FOR_2MIM)
 {
   interval_count=0;
   DEBUG_printf("Time Out\n");
   free(r_mac);
   return 1;  //time out
 }
  free(r_mac);
  return 0;   // no ACK to read
}
int key_test()             //Removing jitter
{
 int i=0;
 int key_times=0;
 for(i=0;i<KEY_TIMES;i++)
 {
  //if((getkey_gpio_46() == false)||(getkey_gpio_26()==false))  //button down
  if((getkey_gpio_26() == false))  //button down
  {
  usleep(20*1000);
  key_times++;
  }
 }
 if(KEY_TIMES==key_times)
  {
	 rg_on();
	 /*
	 while((getkey_gpio_26() == false));
	 for(i = 0; i < 5; i++)
	 {
	 	rg_on();
	 	usleep(100 * 1000);  //100ms
	 	rg_off();
	 	usleep(100 * 1000);  //100ms
	 }
	 */
	 return 1;
  }
  return 0;
}
int main(int argc, char **argv)
{
	int count = 0;
	int state = 0;
	struct timeval start,end;
	DEBUG_printf("monitor-ipsig compile %s--%s\n", __DATE__,__TIME__);
	udp_init();
	while(1)
	{

	 if(1==key_test())  //button down
	 {
	  while(1)
      {
	  DEBUG_printf("Key Down!!!!!\n");
      send_ipmac();
      if(1==check_result())
      break;
      }
	 }
	 else
	  usleep(400*1000);  //500ms
	}
}







