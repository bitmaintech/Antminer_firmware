// 接收端 http://blog.csdn.net/robertkun

//#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>


int main()
{
	setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 

	// 绑定地址
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);
	addrto.sin_port = htons(14235);
	
	// 广播地址
	/*
	struct sockaddr_in from;
	bzero(&from, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	from.sin_port = htons(6000);
	*/
	//send 


	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{   
		printf("socket error");	
		return 0;
	}   
/*
	const int opt = 1;
	//设置该套接字为广播类型，
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		printf("set socket error...");
		return 0;
	}
	*/
///*
	if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1) 
	{   
		printf("bind error...");
		return 0;
	}
//*/
	int len = sizeof(struct sockaddr_in);
	char smsg[100] = {0};

	while(1)
	{
		//从广播地址接受消息
		int ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&addrto,(socklen_t*)&len);
		if(ret<=0)
		{
			printf("read error....");
		}
		else
		{		
		 printf("REV Data: %s\n", smsg);	
		 char *str1=strstr(smsg,",");
		 str1=str1+1;
		 printf("caikai: %s  ",str1);
		 printf("Rev IP is %s  ",inet_ntoa(addrto.sin_addr));
                 printf("Rev Port:%d\n",ntohs(addrto.sin_port));
		
		 
		int ret=sendto(sock, str1,strlen(str1), 0, (struct sockaddr*)&addrto, sizeof(addrto)); 	
		 if(ret<0)
		  {
		  printf("send error....");
		  }
		else
		  {		
		  printf("send respond MAC OK\n");	
		  }
		memset(smsg,0,sizeof(char)*100);
		
	        ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&addrto,(socklen_t*)&len);
		if(ret<=0)
		{
			printf("second read error....");
		}else
		{
		 printf("%s\t", smsg);	 
		 memset(smsg,0,sizeof(char)*100);
		}
		  
		}

		sleep(1);
	}

	return 0;
}
