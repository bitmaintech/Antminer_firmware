#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>

#define DEBUG_P
#define MINER
#ifdef  DEBUG_P
#define DEBUG_printf(format,...) printf(format,  ##__VA_ARGS__)
#else
#define DEBUG_printf(format,...)
#endif

#define MAXLINE 200
#define SERV_PORT 8000
#define COMMAND_SEP '|'
#define PATH_SEP '#'
#define COMMAND_OVER "||"

#ifdef MINER
#define WGET "wget -O /config/update/"
#define SH_PATH "/config/update/"
#else
#define WGET "wget -O /home/fyw/MinerUpdateShell/"
#define SH_PATH "/home/fyw/MinerUpdateShell/"
#endif // MINER

//char *error_info = (char *)malloc(sizeof(char));
char errorinfo[100] = {0};
uint32_t crc32_table[256];
int make_crc32_table()
{
    uint32_t c;
    int i = 0;
    int bit = 0;
    for(i = 0; i < 256; i++)
    {
        c  = (uint32_t)i;
        for(bit = 0; bit < 8; bit++)
        {
            if(c&1)
            {
                c = (c >> 1)^(0xEDB88320);
            }
            else
            {
                c =  c >> 1;
            }
        }
        crc32_table[i] = c;
    }
    return 1;
}

uint32_t make_crc(uint32_t crc, unsigned char *string, uint32_t size)
{
    while(size--)
        crc = (crc >> 8)^(crc32_table[(crc ^ *string++)&0xff]);
    return crc;
}

bool pipe_execution(char * cmd,bool check_flag)
{
    FILE *pPipe;
    char buf[100]={0};
    if ((pPipe = popen(cmd,"r")) == NULL)
    {
        printf("popen failed!\n");
		sprintf(buf, "error:%s\n", strerror(errno));
		return false;
	}
	if(!check_flag)
	{
        pclose(pPipe);
        return true;
	}
	char psBuffer[512] = {0};
    while(fgets(psBuffer, 512, pPipe))
    {
		DEBUG_printf("return values:%s,%d\n", psBuffer,(int)strlen(psBuffer));

        if(strlen(psBuffer) == 2 && strncmp(psBuffer,"0",1) == 0)//the execution is succeed
        {
            pclose(pPipe);
            return true;
        }
        memset(psBuffer,0,512);
    }
    DEBUG_printf("failed at here--------------\n");
	pclose(pPipe);
    return false;
}
bool pipe_cmd_wget(char * path,char * url)//fileinfo is the path and name of the file:MA/minernonitor
{
    char cmd[250] = {0};
    sprintf(cmd,"%s%s %s 2>&1;echo $?",WGET,path,url);
    DEBUG_printf("cmd:%s\n",cmd);
	return pipe_execution(cmd,true);
}
//bool pipe_cmd_stop(char *path,char* url)
//{
//    char cmd[200]={0};
//    strcat(cmd,"`");
//    strcat(cmd,"nohup ");
//    strcat(cmd,SH_PATH);
//    strcat(cmd,path);
//    strcat(cmd,".sh");//stop
//    strcat(cmd," > /dev/null 2>&1");
//    strcat(cmd," &`");
//    DEBUG_printf("cmd:%s\n",cmd);
//	return pipe_execution(cmd,false);
//
//}

bool pipe_cmd_start(char *path,char* url)
{
    char cmd[100] = {0};
    char * appname = strrchr(path,'/');
    appname ++;
    sprintf(cmd,"`%supdate.sh %s &`",SH_PATH,appname);
    DEBUG_printf("cmd:%s\n",cmd);
	return pipe_execution(cmd,false);

}

int pipe_cmd_all_restart()
{
    char cmd[100] = {0};
    strcat(cmd,"`");
    strcat(cmd,SH_PATH);
    strcat(cmd,"update.sh");
    strcat(cmd," &`");
    DEBUG_printf("cmd:%s\n",cmd);
	return pipe_execution(cmd,false);
}

bool Execution(char *cmd_s)
{
    char *url,*buf;
    int n_stop = 0,n_wget = 0,n_start = 0;
    memset(errorinfo,0,100);
    if(strncmp("00",cmd_s,2) == 0)
    {
        buf = cmd_s + 2;
        url = strchr(buf,'#');
        if(!url)
            return false;
        *(url++) = '\0';//(n_stop = pipe_cmd_stop(buf,url)) &&
        if((n_wget = pipe_cmd_wget(buf,url)) && (n_start = pipe_cmd_start(buf,url)))
        {
            strcpy(errorinfo,"succeed!");
            return true;
        }
        else if(n_wget)
        {
            strcpy(errorinfo,"start failed!");
        }
        else
        {
            strcpy(errorinfo,"wget failed!");
        }///use errorinfo when debug to find what's wrong with error_info
        return false;
    }
    else if(strncmp("03",cmd_s,2) == 0)
    {
        if(!pipe_cmd_all_restart())
        {
            strcpy(errorinfo,"all restart failed!\n");
        }
        else
        {
            strcpy(errorinfo,"all restart succeed!\n");
            return true;
        }
    }
    return false;
}
bool CommandParser(char *str_source)
{
    char *cmd_s;//cmd_single
    bool flag = true;
    while((cmd_s = strrchr(str_source,COMMAND_SEP)))
    {
        *(cmd_s ++) = '\0';
        if(!Execution(cmd_s))
        {
            //error_info = (char *)realloc(error_info,strlen(error_info))+strlen(cmd_s)+1);
            //*(error_info + strlen(error_info)) = '\0';
            //strcat(error_info,cmd_s);
            flag = false;
        }
    }
    if(!Execution(str_source))
    {
        //error_info = (char *)realloc(error_info,strlen(str_source)+1);
        //*(error_info + strlen(error_info)) = '\0';
        //strcat(error_info,str_source);
        flag = false;
    }
    return flag;
}
int main(void)//It has not handle the problems of timeout and errors.
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t srccode;
    make_crc32_table();

    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    char *buf;

    int n,cmd_n = 0;
    buf = (char*)malloc(MAXLINE*sizeof(char));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (struct sockaddr *)&servaddr,sizeof(servaddr));
    listen(listenfd, 20);
    printf("Accepting connections ...\n");
    while (1)
    {
        cmd_n = 0;
        uint32_t crc = 0xFFFFFFFF;
        char *cmd = (char *)malloc(sizeof(char));
        *cmd = '\0';
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
        memset(buf,0,MAXLINE);
        DEBUG_printf("cmd0:%s\n",cmd);
        while((n = read(connfd, buf, MAXLINE-1)))
        {
            write(connfd, buf, n);
            DEBUG_printf("buf:%s\n",buf);
            DEBUG_printf("n:%d\n",n);
            cmd_n += n;
            DEBUG_printf("cmd_n:%d\n",cmd_n);
            cmd = (char *)realloc(cmd,cmd_n*sizeof(char));
            DEBUG_printf("cmd1:%s\n",cmd);
            strcat(cmd,buf);
            DEBUG_printf("cmd2:%s\n",cmd);
            memset(buf,0,MAXLINE);
            if(strstr(cmd,COMMAND_OVER))
            {
                break;
            }
        }
        DEBUG_printf("cmd_all:%s\n",cmd);

        char *str_crc = cmd;
        cmd = strchr(str_crc,COMMAND_SEP);
        //cmd +=10;
        *(cmd++) = '\0';
        //cmd_n -= 11;
        cmd_n = strlen(cmd);
        *(cmd+cmd_n-2) = '\0';
        DEBUG_printf("str_crc:%s\n",str_crc);
        crc = make_crc(crc, cmd, strlen(cmd));

        char temp_crc[8]={0};
        sprintf(temp_crc,"0x%x",crc);
        DEBUG_printf("crc:%x\n",crc);
        DEBUG_printf("temp_crc:%s\n",temp_crc);
        if(strcmp(temp_crc,str_crc)!=0)//crc != srccode
        {
            printf("Bad package!======================!\n");
            free(str_crc);
            close(connfd);
            continue;
        }
        DEBUG_printf("cmd_all:%s\n",cmd);
        if(CommandParser(cmd))
        {
            write(connfd, "all succeed!", 12);
            printf("all succeed!\nAccepting connections ...\n");
        }
        else
        {
            write(connfd, "some failed!", 12);
            //write(connfd, error_info, strlen(error_info));
            printf("some failed\nAccepting connections ...\n");
        }
        free(str_crc);
        close(connfd);
    }
    free(buf);
}
