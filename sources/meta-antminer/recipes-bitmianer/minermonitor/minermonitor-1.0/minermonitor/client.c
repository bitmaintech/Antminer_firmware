#include "read_conf.h"
#include "openssl.h"
#include "client.h"
#include "cgminer_api.h"
#include "gen_middata.h"
#include "send_pkg.h"
#include "debug.h"
#include "rev_queue.h"
#include "write_conf.h"
#include "config.h"
#include <pthread.h>
#include <semaphore.h>
#include "cgminer_api.h"
#include "dataexchange.h"
#include "linked_list.h"

PKG_Format package;
PKG_Format deal_package;//receive data
PKG_Format deal_package_cmd;//deal cmd
PKG_Format deal_package_resp;//deal response
int SW=0;
int NO_data=1; //long time  NO data flag
int connect_count;
int wait_res;
char data_time;
sem_t q1_sem;
sem_t q2_sem;
int pthr_rev_id = 0;
bool p_n_over = true;
pthread_t thread_id;
pthread_t thread_rev;
pthread_t thread_deal_cmd_id;
pthread_t thread_deal_response_id;
FILE* fd_pid_conf;//for Singleton .path is pid_lock_dir.


int  auth_flag=0;
//////////////////////////////////////////////////////////////////
////////////////////connect-server////////////////////////////////
void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL)
    {
        DEBUG_printf("A digital certificate information:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        DEBUG_printf("certificate: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        DEBUG_printf("Issued by the: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        DEBUG_printf("No certificate information！\n");
}
int encrypt_connect_server()
{
    //return -1;
    /* SSL initialization*/
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLSv1_client_method());
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    /* creat a new SSL based on ctx */
    ssl = SSL_new(ctx);
    // connect ssl to TCP SOCKET
    SSL_set_fd(ssl, sockfd);
    /* create ssl connection */
    if (SSL_connect(ssl) == -1 || strcmp("(NONE)",SSL_get_cipher(ssl)) == 0)
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    DEBUG_printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);
    return 1;
}

int sock_connect_server()
{
    if(1==SW)
    {
        DEBUG_printf("Web  Switch is OFF!\n");
        exit(1);
    }
    struct hostent *host;
    struct sockaddr_in serv_addr;
    int temp;
    short int port;

    char ip_str[60]={0};
    #ifdef OPENWRT
    strcpy( ip_str,read_luci_conf("ip_server"));
    #endif // OPENWRT
    #ifdef BB_BLACK
    read_conf_by_type(s4_conf_dir,"ip_server",ip_str);
    #endif // BB_BLACK
    char *str=strchr(ip_str,':');
    *(str++)='\0';
    port=atoi(str);
    if ((host = gethostbyname(ip_str)) == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }

    #ifdef PC_IP
    port=2000;
    if ((host = gethostbyname(ip_toconnect)) == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
    #endif
    //socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    //set sockaddr_in strut
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
    //to connect server
    if((temp=connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)))== -1)
    {
        //turnoff_s4(s4_conf_dir);
        //perror("connect failed");
        DEBUG_printf("connect failed\n");
        r_connect = 0;
        return temp;
    }
    r_connect = 1;
    DEBUG_printf("server connected\n");
    return temp;
}


void send_rev_err()
{
    DEBUG_printf("send or recv: Broken pipe\n");
    //exit(0);
}

/////////////////////////////////////////////////////////////////
///////////////////set a TIMER//////////////////////////////////
void set_time(int time1,int time2)
{
    struct itimerval itv;
    itv.it_interval.tv_sec=time1;//first value
    itv.it_interval.tv_usec=0;
    itv.it_value.tv_sec=time2;
    itv.it_value.tv_usec=0;
    setitimer(ITIMER_REAL,&itv,NULL);
}

//////////////////////////////////////////////////////////////////////
/////////////Timer detection and server link is norma/////////////////
void tcp_state()
{
    //DEBUG_printf("tcp_state!------------------------------\n");
    struct tcp_info info;
    int len=sizeof(info);
    if(1== SW )
    {
        DEBUG_printf("Web  Switch is OFF!\n");
        exit(1);
    }
    getsockopt(sockfd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
    if((info.tcpi_state!=TCP_ESTABLISHED))
    {
        DEBUG_printf("Off Line!!! in tcp_state()\n");
        r_connect=0;
        clear_recv(deal_package);
        memset(auth_id,0,70); //clear auth
        if(sock_connect_server()==-1)
        {
            DEBUG_printf("tcp_state, sock_connect_server failed!\n");
        }
        else if(encrypt_connect_server()==-1)
        {
            receivedata = sockreceivedata;
            senddata = socksenddata;

            close(sockfd);
            sock_connect_server();
            DEBUG_printf("encrypt connect failed.\n");
        }
        else
        {
            receivedata = encryptreceivedata;
            senddata = encryptsenddata;
        }
    }
    else
    {
        DEBUG_printf("Connect to normal!!!\n");
    }
}

int recv_serpkg_struct()
{
    //int tmp_time=0;
    int ret=0;
    ret=rev_data_struct();
    if(ret==0)
    {
        DEBUG_printf("OFF LINE !!!!Please Wait!!! in recv_serpkg_struct() in client\n");
        while(1)
        {
            if(r_connect == 0)
            {
                sleep(30);//try to receive data after 30s if it's break
            }
            if(rev_data_struct()>0)
            {
                return 1;
            }
        }
    }
    else if(ret==-1)
    {
        return -1;
    }
    return 1;
}

void signal_check()
{
    set_time(30,30);               //timing 30s
    signal(SIGALRM, tcp_state);    //check the link to server regularly
    signal(SIGPIPE, send_rev_err); //error handling of send and recv
}

void *read_switch(void)
{
    while(p_n_over)
    {
        #ifdef OPENWRT
        if(strcmp("off",read_luci_conf(OPENWRT_CONF_SWITCH))==0)
        {
            SW=1;
        }
        #endif

        #ifdef BB_BLACK
        char key_switch[15];
        if(strcmp("off",read_s4_switch(s4_conf_dir,BB_BLACK_CONF_SWITCH,key_switch))==0)
        {
            SW=1;
        }
        printf("key_switch = %s\n",key_switch);
        #endif
        sleep(15);
    }
    DEBUG_printf("will bi exited###################\n");
    //exit(1);
}

void *pthr_rev()
{
    while(p_n_over)
    {
        if((Q_cmd.rear+1)%MAXQSIZE!=Q_cmd.front && (Q_respond.rear+1)%MAXQSIZE!=Q_respond.front)
        {
            if(-1==recv_serpkg_struct())
            {
                DEBUG_printf("Bad Package REV!\n");
                sleep(1);
                continue;
            }
            if(package.pkg_cmd[0]!='f')
            {
                DEBUG_printf("this is command\n");
                EnQueue(&Q_cmd,package);
                //sem_post(&q1_sem);
            }
            else
            {
                DEBUG_printf("this is respond \n");
                EnQueue(&Q_respond,package);
                //sem_post(&q2_sem);
            }
        }
        else
        {
            DEBUG_printf("the queue is full\n");
            sleep(5);
        }
    }
}

void *pthr_deal_cmd()
{
    while(p_n_over)
    {
        DEBUG_printf("next command!\n");
        if(1==SW)
        {
            DEBUG_printf("Web  Switch is OFF!\n");
            exit(1);
        }
        if(DeQueue(&Q_cmd,&deal_package_cmd))
        {
            DEBUG_printf("get a command head:%s!\n",deal_package_cmd.pkg_head);
            DEBUG_printf("get a command:%s!\n",deal_package_cmd.pkg_cmd);

            QueueTraverse(Q_cmd,visit);
            if(!deal_cmd(deal_package_cmd))
                continue;
            DEBUG_printf("after deal_cmd()\n");
            DEBUG_printf("get a command nonce:%s!\n",deal_package_cmd.pkg_nonce);

            NO_data=0;//data recived
            Insert(&deal_package_cmd,L_cmd,L_cmd);

            printf("new node:%s,%s==========\n",deal_package_cmd.pkg_cmd,deal_package_cmd.pkg_nonce);
            clear_recv(deal_package_cmd);
        }
        else
        {
            DEBUG_printf("queue empty1\n");
            sleep(1);
        }
    }
}


void *pthr_deal_response()
{
    while(p_n_over)
    {
        if(0!=NUL_Queue(&Q_respond))
        {
            wait_res=0;
            if(DeQueue(&Q_respond,&deal_package_resp))
            {
                QueueTraverse(Q_respond,visit);
                rev_respond();
                printf("response node:%s,%s----------\n",deal_package_resp.pkg_cmd,deal_package_resp.pkg_nonce);
                Position curnode = Find(&deal_package_resp,L_cmd);
                if(curnode == NULL)
                {
                    printf("no cmd for responding!\n");
                    exit(1);
                }
                if(strcmp(curnode->item->pkg_cmd,"01") == 0)
                {
                    #ifdef BB_BLACK
                    write_by_type(s4_conf_dir,deal_package_resp.pkg_cmd);
                    #endif // BB_BLACK
                    #ifdef OPENWRT
                    write_s3_auth(deal_package_resp.pkg_cmd);
                    #endif // OPENWRT
                }
                printf("Delete node:%s,%s----------\n",deal_package_resp.pkg_cmd,deal_package_resp.pkg_nonce);
                Delete(&deal_package_resp,L_cmd);
                clear_recv(deal_package_resp);
            }
            else
            {
                DEBUG_printf("respond queue  empty\n");
            }
            NO_data=0;//data recived
        }
    }
}

void set_dconfig_flag()
{
    if(MINERTYPE == 5)
    {
        write_s4_respond(s4_conf_dir,"fx");
    }
    else if(MINERTYPE == 3)
    {
        write_s3_auth("fx");
    }
}
void Initsem()
{
    int ret;
    ret= sem_init(&q1_sem, 0, 0);
    if(ret != 0)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    ret= sem_init(&q2_sem, 0, 0);
    if(ret != 0)
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
}
void mask_sig()
{
    sigset_t newmask;
    sigemptyset(&newmask);                 // clear
    sigfillset(&newmask);
    sigdelset(&newmask, SIGINT);          //without SIGINT
    sigdelset(&newmask, SIGPIPE);          //without SIGINT
}

void sig_handle(int signum)
{
    DEBUG_printf("receive signal :%d\n",signum);
    //SIGUSR1:pause;SIGUSR2:start;
    if(signum == SIGUSR1)
    {
        set_time(6000000,6000000);
        r_connect = 0;
        close(sockfd);
        ClearQueue(&Q_cmd);//clear the comdmand queue.
        ClearQueue(&Q_respond);
    }
    else if(signum == SIGUSR2)
    {
        ClearQueue(&Q_cmd);//clear the comdmand queue.
        ClearQueue(&Q_respond);
        set_time(30,30);
        tcp_state();
    }
}
void init_daemon()
{
    int pid;
    int i;
    pid=fork();
    if(pid<0)
        exit(1);  //creation error,exit
    else if(pid>0) //parrent process exit
        exit(0);

    setsid(); //make the son process be group leader
    pid=fork();
    if(pid>0)
        exit(0); //exit again,make it be not group leader so the process won't open the termination
    else if(pid<0)
        exit(1);

    //close the handle opened by the process.
    for(i=0;i<NOFILE;i++)
        close(i);
    chdir("/root/test");  //change the directory
    umask(0);//reset the mask
    return;
}

void Stop(int signo)
{
    printf("oops! stop by ctrl+c or kill!!!----------------------------------------\n");
    if(flock(fileno(fd_pid_conf),LOCK_UN) != 0)
    {
        DEBUG_printf("unlock fd_pid_conf failed!\n");
    }
    else
    {
        DEBUG_printf("unlock fd_pid_conf succeed!\n");
    }
    fclose(fd_pid_conf);

    DeleteList(L_cmd);

    p_n_over = false;
    //sleep(5);
    pthread_join(thread_id,NULL);
    //pthread_join(thread_rev,NULL);
    pthread_join(thread_deal_cmd_id,NULL);
    pthread_join(thread_deal_response_id,NULL);

    DestroyQueue(&Q_cmd);
    DestroyQueue(&Q_respond);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);

    CONF_modules_free();
    ERR_remove_state(0);
    ENGINE_cleanup();
    CONF_modules_unload(1);
    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
    exit(0);
}

bool goon()
{
    extern int errno;
    int pid;
    if(access(pid_lock_dir,F_OK))//not exist
    {
        //mkdir
        char *dir_temp = (char*)malloc(strlen(pid_lock_dir));
        strcpy(dir_temp,pid_lock_dir);
        char *tempfont,*tempback;
        tempfont = dir_temp+1;
        while(tempfont && (tempback = strchr(tempfont,'/')))
        {
            *(tempback) = '\0';
            if(access(dir_temp,F_OK))
            {
                mkdir(dir_temp,0777);
            }
            *(tempback) = '/';
            tempfont = ++tempback;
        }
        //end of mkdir
        //new
        fd_pid_conf = fopen(pid_lock_dir,"w");
        if(fd_pid_conf)
        {
            DEBUG_printf("created!\n");
        }
        //lock
        if((fd_pid_conf!=NULL)&&(flock(fileno(fd_pid_conf),LOCK_EX)==0))
        {
            //getpid and write
            pid = getpid();
            fprintf(fd_pid_conf,"The thread executing miner-monitor:%d",pid);
            DEBUG_printf("locked!\n");
        }
        return true;
    }
    fd_pid_conf = fopen(pid_lock_dir,"w");
    if((fd_pid_conf!=NULL)&&(flock(fileno(fd_pid_conf),LOCK_EX|LOCK_NB)==0))//not lock and lock
    {
        //getpid and write
        pid = getpid();
        fprintf(fd_pid_conf,"The thread executing miner-monitor:%d",pid);
        DEBUG_printf("exist,write and lock!\n");
        return true;
    }
    fclose(fd_pid_conf);
    return false;
}
int main(int argc, char *argv[])
{
    if(!goon())
    {
        printf("process exist!\n");
        return 1;
    }
    #ifndef DEBUG_P
    //init_daemon();
    #endif // DEBUG_P
    int ret = 0;
    int  auth_flag=0;
    if(argc>1)
    {
        sleep(40);
    }
    mask_sig();
    //Initsem();
    InitQueue(&Q_cmd,&Q_respond);
    MakeEmpty(L_cmd);//init linked list

    set_dconfig_flag();           //set the default auth flag in file
    ret = pthread_create(&thread_id, NULL, (void*)read_switch, NULL);//read web  config switch
    if (ret)
    {
        DEBUG_printf("Create pthread error!\n");
        return 1;
    }
    sleep(1);
    if(1==SW)
    {
        DEBUG_printf("Web  Switch is OFF!\n");
        exit(1);
    }
    while(sock_connect_server() == -1)
    {
        sleep(30);
    }
    if(encrypt_connect_server()==-1)
    {
        receivedata = sockreceivedata;
        senddata = socksenddata;
        DEBUG_printf("encrypt connect failed.\n");
        close(sockfd);
        sock_connect_server();
    }
    else
    {
        receivedata = encryptreceivedata;
        senddata = encryptsenddata;
    }
    signal_check(30);
    signal(SIGUSR1,sig_handle);
    signal(SIGUSR2,sig_handle);
    signal(SIGINT, Stop);
    signal(SIGTERM, Stop);
    pthr_rev_id = pthread_create(&thread_deal_cmd_id,NULL,(void*)pthr_deal_cmd,NULL);
    if (pthr_rev_id)
    {
        DEBUG_printf("Create pthread error!\n");
        return 1;
    }
    pthr_rev_id = pthread_create(&thread_deal_response_id,NULL,(void*)pthr_deal_response,NULL);
    if (pthr_rev_id)
    {
        DEBUG_printf("Create pthread error!\n");
        return 1;
    }
    //receive the data
    while(p_n_over)
    {
        if((Q_cmd.rear+1)%MAXQSIZE!=Q_cmd.front && (Q_respond.rear+1)%MAXQSIZE!=Q_respond.front)
        {
            if(-1==recv_serpkg_struct())
            {
                DEBUG_printf("Bad Package REV!\n");
                sleep(1);
                continue;
            }
            if(package.pkg_cmd[0]!='f')
            {
                DEBUG_printf("this is command\n");
                EnQueue(&Q_cmd,package);
            }
            else
            {
                DEBUG_printf("this is respond \n");
                EnQueue(&Q_respond,package);
            }
        }
        else
        {
            DEBUG_printf("the queue is full\n");
            sleep(5);
        }
    }
    return 1;
}





