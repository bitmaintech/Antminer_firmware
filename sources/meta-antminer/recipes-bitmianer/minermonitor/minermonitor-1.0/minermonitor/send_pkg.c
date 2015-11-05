#include "send_pkg.h"
#include "debug.h"
#include "config.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "dataexchange.h"
#include "rev_queue.h"
char *ip_pc="192.168.1.113";

char buf[BUFFER_SIZE];//the buffer between miner and server
char serv_v[3];       //receive the start 2 bytes from server
char serv_cmd[3];     //the command of server
char body_len[2];     //the length of server packet
char auth_id[70];     //store the cuniqid of miner and server when certificating
int sockfd;
SSL_CTX *ctx;
SSL *ssl;
int sendbytes;
int recvbytes; //丿
PKG_Format package;
PKG_Format deal_package;
PKG_Format deal_package_cmd;//deal cmd
PKG_Format deal_package_resp;//deal response

GETS4_CONFIG get_cgminer_conf;
GETS3_CONFIG gets3_cgminer_conf;
POOLS_D_COUNT changed_pools;
char rev_ID[40];

///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////
////////////clear the data received before//////////////
void clear_recv(PKG_Format pkg_struct)
{
    memset(serv_v,0,3);
    memset(serv_cmd,0,3);
    memset(body_len,0,2);
    memset(buf,0,BUFFER_SIZE);

    memset(pkg_struct.pkg_head,0,3);
    memset(pkg_struct.pkg_cmd,0,3);
    memset(pkg_struct.pkg_nonce,0,7);
    memset(pkg_struct.pkg_body,0,BUFFER_SIZE);
    memset(rev_ID,0,40);
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
///////////Receive the server data and adversary analytical and judgment/////////////

int  rev_data_struct()
{
    char buff[100];
    memset(buff,0,100);
    DEBUG_printf("r_connect = %d.\n",r_connect);
    if((recvbytes = receivedata(ssl,sockfd,&package.pkg_head,2)) == -1)         //At the beginning of two bytes
    {
        DEBUG_printf("recv data failed1.\n");
        r_connect = 0;//the failure of receiving data prove the break of connection,mark it at here.
        clear_recv(deal_package);
        ClearQueue(&Q_cmd);//clear the comdmand queue when failed to receive data.
        ClearQueue(&Q_respond);//clear the comdmand queue when failed to receive data.
        return 0;
        //perror("recv");
        //exit(1);
    }
    DEBUG_printf("recv package.pkg_head:%s.\n",package.pkg_head);
    package.pkg_head[recvbytes]='\0';
    if(recvbytes==0)
    {
        DEBUG_printf("the first two byte not 00   it is 0\n");
        return 0;
    }
    else if(strcmp(package.pkg_head,"00")!=0)
    return -1;  //bad package
    if((recvbytes = receivedata(ssl,sockfd,&package.pkg_cmd,2)) == -1) //Command two bytes
    {
        r_connect = 0;
        return 0;
        //perror("recv");
        //exit(1);
    }
    if(recvbytes==0)
        return 0;
    else if(recvbytes!=2)
        return -1; //bad package
    package.pkg_cmd[recvbytes]='\0';
    DEBUG_printf("rev from server cmd = \n%s\n",package.pkg_cmd);
    unsigned char nonce_len[2];
    if ((recvbytes = receivedata(ssl,sockfd,&nonce_len,2)) == -1)  //two bytes of the lenth of body
    {
        r_connect = 0;
        return 0;
        //perror("recv");
        //exit(1);
    }
    if(recvbytes==0)
        return 0;
    else if(recvbytes!=2)
        return -1; //bad package
    int sum;
    //DEBUG_printf("pkg_body_len[0]=%c\n",package.pkg_body_len[0]);
    //DEBUG_printf("pkg_body_len[1]=%c\n",package.pkg_body_len[1]);
    sum=nonce_len[0]*256+nonce_len[1];                              // Calculate the length of the body
    DEBUG_printf("rev from server sum=\n%d\n",sum);
    //memset(package.pkg_body,0,BUFFER_SIZE);
    if(sum==0)
    {
        package.pkg_nonce[0]='\0';
    }
    else
    {
        if ((recvbytes = receivedata(ssl,sockfd,&package.pkg_nonce,sum)) == -1)  //According to the receiving body length
        {
            r_connect = 0;
            return 0;
            //perror("recv");
            //exit(1);
        }
        package.pkg_nonce[sum]='\0';
        DEBUG_printf("recvbytes:%d,sum:%d,pkgnonce=%s\n",recvbytes,sum,package.pkg_nonce);
        if(recvbytes==0)
        {
            r_connect = 0;
            return 0;
        }
        else
        {
            if(recvbytes!=sum)                                 //Judge whether the body of the received meet the requirements
            {
                DEBUG_printf("PAKGE ERROR!!!!!\n");
                return -1;
                //exit(1);
            }
        }
    }
    return 1;
}//*/


/////////////////////////////////////////////////
//////////////rev_respond///////////////////////
//////////Receive the server returned data for the second time////////////////
void rev_respond()
{
    char *res;
    //if(0==rev_data())
    res=deal_package_resp.pkg_cmd;
    //Determine the server response signal，
    if(strcmp(res,"f1")==0)
    {
        DEBUG_printf("Server respond  F1\n\n");
        exit(0);
    }
    else if(strcmp(res,"f2")==0)
    {
        DEBUG_printf("Server respond  F2\n\n");
        exit(0);
    }
    else if(strcmp(res,"f3")==0)
    {
        DEBUG_printf("Server respond  F3\n\n");
        exit(0);
    }
    else if(strcmp(res,"f4")==0)
    {
        DEBUG_printf("Server respond  F4\n\n");
        exit(0);
    }
    else if(strcmp(res,"f5")==0)
    {
        DEBUG_printf("Server respond  F5\n\n");
    }
    else if(strcmp(res,"f6")==0)
    {
         DEBUG_printf("Server respond  F6\n\n");
         DEBUG_printf("F6 exit!!!!!!\n");
         exit(0);

    }
    else if(strcmp(res,"f7")==0)
    {
        DEBUG_printf("Server respond  F7\n\n");
        exit(0);
    }
    else
    {
        DEBUG_printf("respond rev : %s",res);
        exit(0);
    }
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
///////6 bytes each time before sending to the server to send all need////////////////
char send_head(char *cmd,unsigned int body_len)
{
    unsigned char body_len1;
    unsigned char body_len2;
    body_len1=body_len/256;
    DEBUG_printf("body_len1=%d\n",body_len1);
    body_len2=body_len%256;
    DEBUG_printf("body_len2=%d\n",body_len2);
    //Packg inpackg 6 byte;
    if ((sendbytes = senddata(ssl,sockfd,"00",sizeof("00")-1)) == -1)
    {
        perror("send head");
        exit(1);
    }//perror("send");
    if ((sendbytes = senddata(ssl,sockfd,cmd,strlen(cmd))) == -1)
    {
        perror("send");
        exit(1);
    }//perror("send cmd");
    //DEBUG_printf("sizeof(cmd)=%d\n",strlen(cmd));
    if ((sendbytes = senddata(ssl,sockfd,&body_len1, sizeof(char))) == -1)
    {
        perror("send");
        exit(1);
    }
    if ((sendbytes = senddata(ssl,sockfd,&body_len2, sizeof(char))) == -1)
    {
        perror("send");
        exit(1);
    }
    return 1;
}

void send_connect_auth(char *cmd)
{
    char *total_packg=NULL;
    char *userid;
    char *mac_data;
    char *ip_data;
    char ip_temp[40];
    char *cuniqid;
    char uid_tmp[60];
    char type[10];
    unsigned char authbody_size;
    memset(uid_tmp,0,sizeof(char)*60);
    memset(type,0,sizeof(char)*10);
    memset(ip_temp,0,sizeof(char)*40);
    cuniqid= (char*)calloc(10,sizeof(char));
    mac_data=(char*)calloc(30,sizeof(char));
    ip_data=(char*)calloc(25,sizeof(char));
    userid=(char*)calloc(60,sizeof(char));

    if(!get_ip(ETH,ip_temp))
    {
        DEBUG_printf("get ip error!\n");
    }
    sprintf(ip_data,"IP=%s,",ip_temp);

    strcpy( userid,"UID=");
    #ifdef BB_BLACK
    strcat( userid,read_conf_by_type(s4_conf_dir,"api-uid",uid_tmp));
    #endif // BB_BLACK
    #ifdef OPENWRT
    strcat( userid,read_luci_conf("user_id"));
    #endif // OPENWRT

    strcat( userid,",");
    sprintf(type,"TYPE=%d",MINERTYPE);
    authbody_size=strlen(gen_cuniqid(deal_package_cmd.pkg_nonce))+strlen(get_mac(mac_data,ETH))+strlen(ip_data)+strlen(userid)+strlen(type);
    send_head(cmd,authbody_size);

    total_packg=(char*)calloc((authbody_size+4),sizeof(char));
    strcpy(total_packg,deal_package_cmd.pkg_nonce);
    strcat(total_packg,mac_data);
    strcat(total_packg,ip_data);
    strcat(total_packg,userid);
    strcat(total_packg,type);
    if ((sendbytes = senddata(ssl,sockfd,total_packg,authbody_size)) == -1)
    {
        perror("send");
        //exit(1);
    }
    DEBUG_printf("send pakge str :%s\n",total_packg);

    free(mac_data);
    free(ip_data);
    free(userid);
    free(cuniqid);
    if(total_packg)
    free(total_packg);
}

////////////////////////////////////////////////////
////////////Send CONNECT_CC/////////////////////////
int send_connect_cc(char *cmd)
{
    int revbody_size=strlen(buf);
    DEBUG_printf("02revbody_size=%d\n",revbody_size);
    send_head(cmd,revbody_size);
    if ((sendbytes = senddata(ssl,sockfd,buf, revbody_size)) == -1)
    {
        perror("send");
        exit(1);
    }
    DEBUG_printf("send what I rev:\n%s\n",buf);
    return 1;
}


/////////////////////03--30--03--30////////////////////////////
///////////////////get_status_res//////////////////////////////
////////////get  cgminer  state&& send////////////////////
int send_getstatus_res(char *cmd_server_code)
{
    char *api_len=NULL;
    char *buf_data=NULL;
    char *cuniqid;
    int len,i;
    short int port;
    int statebody_size=0;
    char ipaddr[40];
    char *cmd_server;
    char *s4_dataformat_dir;
    char *api_command;
    for(i=0;i<COMMAND_SUM;i++)
    {
        if(strcmp(cmd_server_code,server_command_code[i])==0)
        {
        	len = strlen(dataformat_conf_dir[i]) + 1;
            s4_dataformat_dir =(char *)malloc(len * sizeof(char));
            memset(s4_dataformat_dir,0,len);
            strcpy(s4_dataformat_dir,dataformat_conf_dir[i]);

        	len = strlen(server_command[i]) + 1;
            api_command = (char *)malloc(len * sizeof(char));
            memset(api_command,0,len);
            strcpy(api_command,server_command[i]);

        	len = strlen(command_code[i]) + 1;
            cmd_server = (char *)malloc(len * sizeof(char));
            memset(cmd_server,0,len);
            strcpy(cmd_server,command_code[i]);
            break;
        }
    }

    buf_data=(char*)calloc(4096,sizeof(char));
    //cuniqid= (char*)calloc(10,sizeof(char));
    len=strlen(gen_cuniqid(deal_package_cmd.pkg_nonce));

    //Get_Port
    //Get_IP
    #ifdef OPENWRT
    port=4028;
    #endif // OPENWRT
    #ifdef BB_BLACK
    char port_str[10];
    port=atoi(read_s4_switch(s4_conf_dir,"port_local",port_str));
    #endif // BB_BLACK

    if(!get_ip(ETH,ipaddr))
    {
        DEBUG_printf("get IP error\n");
    }
    #ifdef PC_IP
    port=4028;
    strcpy(ipaddr,ip_pc);
    #endif

    api_len=callapi(api_command, ipaddr,port);

    if(strlen(api_len) == 0)
    {
        DEBUG_printf("nothing returned form callapi\n\n\n ");
        return 0;
    }
    //DEBUG_printf("the result of callapi is %s\n",api_len);
    DEBUG_printf("result of callapi:%s\n ",api_len);
    statebody_size=strlen(data_format(s4_dataformat_dir,buf_data,api_len));

    DEBUG_printf("state_len=%d\n",(statebody_size+len));
    send_head(cmd_server,(statebody_size+len));


    if ((sendbytes = senddata(ssl,sockfd,deal_package_cmd.pkg_nonce, len)) ==-1)
    {
        perror("send");
    }

    if ( (sendbytes =senddata(ssl,sockfd,buf_data, (statebody_size))) ==-1)
    {
        perror("send");
    }
    DEBUG_printf("sendbytes=%d\n",sendbytes);
    DEBUG_printf("send pakge str :%s\n",buf_data);

    if(cmd_server)
        free(cmd_server);
    if(s4_dataformat_dir)
        free(s4_dataformat_dir);
    if(api_command)
        free(api_command);
    if(api_len)
        free(api_len);
    free(buf_data);
    //free(cuniqid);
    return 1;
}


//////////////////////////////////////////////////////Cgminer connection attempts to restart after 5 seconds, if the connection fails, wait 5 seconds, trying to connect to 5 times
////////////Set up the parameter of miner and pool//////////////////////////////
//write  cgminer.conf
int write_config_s4(PKG_Format pkg_struct)
{
    GETS4_CONFIG *new_pools_data;
    new_pools_data=creat_newpools(rev_pools(pkg_struct.pkg_body,&changed_pools),save_old_conf(s4_cgc_dir,&get_cgminer_conf));

    create_conf(new_pools_data,s4_cgc_dir);
    //restart_cgminer();
    return 1;
}
int write_config_s3(PKG_Format pkg_struct)
{
   GETS3_CONFIG *new_pools_data;
   new_pools_data=creat_s3_newpools(rev_pools(pkg_struct.pkg_body,&changed_pools),save_s3_webconf(&gets3_cgminer_conf));
   write_S3_conf(new_pools_data);
   //restart_cgminer();
   return 1;
}

char restart_cgminer()  //restart  cgminer
{
    struct hostent *ip;
    struct sockaddr_in serv;
    int sock;
    char ipaddr[40];

    short cg_port;
    #ifdef OPENWRT
    cg_port=4028;
    #endif // OPENWRT
    #ifdef BB_BLACK
    char port_str[10];
    cg_port=atoi(read_s4_switch(s4_conf_dir,"port_local",port_str));
    #endif // BB_BLACK

    if(!get_ip(ETH,ipaddr))
    {
        DEBUG_printf("get IP error\n");
    }
    #ifdef PC_IP
    cg_port=4028;
    strcpy(ipaddr,ip_pc);
    #endif

    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs,SIGPIPE);
    sigprocmask(SIG_BLOCK,&sigs,0);
    if(5 == MINERTYPE)
    {
        system("/etc/init.d/cgminer.sh restart");
    }
    else
    {
        system("/etc/init.d/cgminer restart");
    }

    #ifdef PC_IP
    system("/etc/init.d/cgminer.sh restart");
    #endif

    sigprocmask(SIG_UNBLOCK,&sigs,0);
    sleep(5);
    ip = gethostbyname(ipaddr);
    if (!ip) {
        DEBUG_printf("Failed to resolve host \n");
        return 0;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        DEBUG_printf("Socket initialisation failed: %s\n", strerror(errno));
        return 0;
    }
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr = *((struct in_addr *)ip->h_addr);
    serv.sin_port = htons(cg_port);
    int i=6;
    while (connect(sock, (struct sockaddr *)&serv, sizeof(struct sockaddr))<0 && (i--)>0)
    {
        DEBUG_printf("Cgminer!Socket connect failed: %s\n", strerror(errno));
        DEBUG_printf("restart not well!!\nwait next 5s......\n");
        sleep(5);
    }
    close(sock);
    if(i<=0)
    {
        DEBUG_printf("restart timeout!\n");
        return -1;
    }
    else
    {
        DEBUG_printf("restart OK!\n");
        return 1;
    }
}

// After reboot is complete, client send SET_CONFIG_RES
void send_setconfig_result(char *cmd,char *conf_stat)
{
    char *cuniqid;
    char *buf_data;
    int body_len;
    cuniqid= (char*)calloc(10,sizeof(char));
    buf_data=(char*)calloc(60,sizeof(char));
    strcpy(buf_data,gen_cuniqid(deal_package_cmd.pkg_nonce));
    strcat(buf_data,"ID=");
    strcat(buf_data,rev_ID);
    strcat(buf_data,",result=");
    strcat(buf_data,conf_stat);
    body_len=strlen(buf_data);
    send_head(cmd, body_len);
    if((sendbytes = senddata(ssl,sockfd,buf_data, body_len)) == -1)
    {
        perror("send");
        //exit(1);
    }
    DEBUG_printf("send cgminer state :%s\n",buf_data);
    //free(cuniqid);
    free(buf_data);
}

////////////////////////////////////////////////////
//////////////upgrade Cgminer////////////////////////////
//get url address by libcurl
void curl_get()
{
    CURL *curl;
    FILE *fp;
    size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        int written = fwrite(ptr, size, nmemb, (FILE *)fp);
        return written;
    }
    curl_global_init(CURL_GLOBAL_ALL);
    curl=curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "www.baidu.com");

    if((fp=fopen("save.txt","w"))==NULL)
    {
        curl_easy_cleanup(curl);
        exit(1);
    }
    ////CURLOPT_WRITEFUNCTION The action of the subsequent to write_data processing function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
}

int updata()     //todo
{
     /*
     * Parse the received data in the command
     * ………………………………………
     */
    return 1;
}


int deal_cmd(PKG_Format pkg_struct)
{
    if(strcmp(pkg_struct.pkg_cmd,"01") == 0)
    {
        send_connect_auth("10");   //All response to server command
    }
    else if(strcmp(pkg_struct.pkg_cmd,"06") == 0)
    {
        #ifdef OPENWRT
        write_config_s3(pkg_struct);
        #endif // OPENWRT
        #ifdef BB_BLACK
        write_config_s4(pkg_struct);
        #endif // BB_BLACK

        if(restart_cgminer()==1)
        {
            send_setconfig_result("60","S");
            DEBUG_printf("restart cgminer OK OK \n");
        }
        else
        {
            send_setconfig_result("60","F");
            DEBUG_printf("restart cgminer FAILD \n");
            return 0;
        }
    }
    else if(send_getstatus_res(pkg_struct.pkg_cmd)==0)
    {
        DEBUG_printf("after send_getstatus_res()\n");
        //clear_recv(pkg_struct);
        return 0;
    }
    clear_recv(pkg_struct);
    return 1;
}


