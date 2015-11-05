#include "cgminer_api.h"
#include "debug.h"
#include "config.h"
#include "cjson.h"
#define MAXQSIZE_FORMAT 50

//read the config file for dataformat
int read_confdata(char *s4_dataformat_dir,char ***confa,char ***confb)
{
    int i,len;
    char* pContent;

    FILE* fp = fopen(s4_dataformat_dir, "rb+");
    if(fp==NULL)
    {
     DEBUG_printf("open error0\n");
     return 0;
    }
    fseek(fp,0,SEEK_END);
    len=ftell(fp);
    if(0 == len)
    {
        return 0;
    }
    fseek(fp,0,SEEK_SET);
    pContent = (char*) malloc(sizeof(char)*len+100);
    fread(pContent,1,len,fp);
    fclose(fp);
    cJSON *jsonroot = cJSON_Parse(pContent);
    int sum = cJSON_GetArraySize(jsonroot);
    *confa = (char**)malloc(sum*sizeof(char*));
    *confb= (char**)malloc(sum*sizeof(char*));
    int lena,lenb;
    for(i =0;i<sum;i++)
    {
        lena = strlen(cJSON_GetArrayItem(jsonroot,i)->string);
        lenb = strlen(cJSON_GetArrayItem(jsonroot,i)->valuestring);
        (*confa)[i] = (char*)malloc(sizeof(char)*(lena+1));
        (*confb)[i] = (char*)malloc(sizeof(char)*(lenb+1));
        strcpy((*confa)[i],cJSON_GetArrayItem(jsonroot,i)->string);
        strcpy((*confb)[i],cJSON_GetArrayItem(jsonroot,i)->valuestring);
        (*confa)[i][lena] = '\0';
        (*confb)[i][lenb] = '\0';
    }
    cJSON_Delete(jsonroot);
    free(pContent);
    return sum;
}
//data handling,switch the source data to new format and send to server.
char *data_format(char *s4_dataformat_dir,char *buf_data,char *buf_tmp)
{
    int i;
    char sep[2];
    char **confa = NULL;
    char **confb = NULL;
    int sum = read_confdata(s4_dataformat_dir,&confa,&confb);
    DEBUG_printf("sum = %d\n",sum);

    char *begin,*comma,*equ,*temp1,*temp2;
    char tmp[100];
    memset(tmp,'\0',100);
    begin = strchr(buf_tmp,SEPARATOR);
    if(begin != NULL)
    {
        *(begin++) = '\0';
    }
    temp1 = strrchr(buf_tmp,SEPARATOR);
    if(temp1 != NULL)
    {
        *temp1 = '\0';
    }
    while(begin != NULL && *begin != 0)
    {
        temp1 = strchr(begin,COMMA);
        temp2 = strchr(begin,SEPARATOR);
        comma = (temp1 == NULL || temp2 < temp1) ? temp2 : temp1;
        strncpy(sep,comma,1);
        if(comma != NULL)
        {
            *(comma++) = '\0';
        }
        equ = strchr(begin,EQ);
        if(equ != NULL)
        {
            *(equ++) = '\0';
        }
        for(i = 0;i < sum;i ++)
        {
            if(strcmp(confa[i],begin)==0)
            {
                sprintf(tmp,"%s=%s", confb[i],equ);
                strcat(buf_data,tmp);
                if(comma != NULL && *comma  != 0)
                {
                    strncat(buf_data,sep,1);
                }
                break;
            }
        }
        begin = comma;
    }
    int leng = strlen(buf_data);
    if(buf_data[leng-1] == ',' || buf_data[leng-1] == '|')
    {
        buf_data[leng-1] = '\0';
    }
    for(i = 0;i<sum;i++)
    {
        if(confa[i])
            free(confa[i]);
        if(confb[i])
            free(confb[i]);
    }
    if(confa)
        free(confa);
    if(confb)
        free(confb);
    DEBUG_printf("after format :%s\n\n",buf_data);
    return buf_data;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////get cgminer api/////////////////////////////////
char *callapi(char *api_command, char *host, short int port)
{
//test at computer
//char *nhost = "10.12.60.8";
//port = 4028;
//strcpy(host,nhost);
//printf("host:%s\n port:%d\n\n",host,port);
    struct hostent *ip;
    struct sockaddr_in serv;
    int sock;
    int break_flag = 0;
    int n, p;
    int count=0;
    size_t bufsz = CG_RECVSIZE;
    char *buf = malloc(bufsz + 1);
    assert(buf);
    ip = gethostbyname(host);
    if (!ip) {
        DEBUG_printf("Failed to resolve host %s\n", host);
        return NULL;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        DEBUG_printf("Socket initialisation failed: %s\n", strerror(errno));
        return NULL;
    }
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr = *((struct in_addr *)ip->h_addr);
    serv.sin_port = htons(port);

    //After restart  Try to establish connection with cgminer
    while(connect(sock, (struct sockaddr *)&serv, sizeof(struct sockaddr))<0)
    {
        if(break_flag == 0)
        {
            DEBUG_printf("Call API error.Disconnect with server!\n");

            raise(SIGUSR1);
            break_flag = 1;
        }
        DEBUG_printf("Call API Failed wait 5s !!\n");

        sleep(5);
    }
    if(break_flag)
    {
        close(sock);
        raise(SIGUSR2);
        return "";
    }
    n = send(sock, api_command, strlen(api_command), 0);
    if (n<0) {
        DEBUG_printf("Send failed: %s\n", strerror(errno));
    }
    else {
        DEBUG_printf("Send succed\n");
        p = 0;
        buf[0] = '\0';
        while (1) {
            if (bufsz < CG_RECVSIZE + p) {
            bufsz *= 2;
            buf = realloc(buf, bufsz);
            assert(buf);
            }
            DEBUG_printf("begin to Recv\n");
            n = recv(sock, &buf[p], CG_RECVSIZE, 0);

            DEBUG_printf("Recved n: %d\n", n);
            if (n<0) {
            DEBUG_printf("Recv failed: %s\n", strerror(errno));
            break;
            }
            if (n == 0)
            break;
            p += n;
            buf[p] = '\0';
        }
    }
    close(sock);
    DEBUG_printf("the result of callapi about %s: %s\n\n",api_command, buf);
    return buf;
}



