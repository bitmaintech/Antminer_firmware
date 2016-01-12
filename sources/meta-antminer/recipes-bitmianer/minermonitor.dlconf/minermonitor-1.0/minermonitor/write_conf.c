#include "write_conf.h"
#include "read_conf.h"
#include "config.h"
char rev_ID[40];
char *luci_cgminer_dir;
////////////////////////////////////////////////////////////////////////
//////////////////type is the version of miner//////////////////////////////////////
char *read_conf_by_type(char* fileName,char *key,char *value)
{
    #ifdef BB_BLACK
    read_bb_black_switch(fileName,key,value);
    #endif // BB_BLACK
    #ifdef OPENWRT
    strcpy(value,read_luci_conf(key));
    #endif
    return value;
}

////////////////////////////////////////////////////////////////////////
//////////////////S4 miner//////////////////////////////////////
char *read_bb_black_switch(char* fileName,char *key,char *value)
{
    long len;
    char* pContent;
    FILE* fp = fopen(fileName, "rb");
    if(fp==NULL)
    {
        DEBUG_printf("open error!\n");
    }
    fseek(fp,0,SEEK_END);
    len=ftell(fp);
    if(0 == len)
    {
        return NULL;
    }
    fseek(fp,0,SEEK_SET);
    pContent = (char*) malloc(sizeof(char)*len+100);
    fread(pContent,1,len,fp);
    fclose(fp);
    cJSON *jsonroot = cJSON_Parse(pContent);
    strcpy(value,cJSON_GetObjectItem(jsonroot,key)->valuestring);
    cJSON_Delete(jsonroot);
    free(pContent);
    return value;
}

int write_by_type(char* fileName,char *rev)
{
    if(MINERTYPE == 3)
    {
        return write_openwrt_auth(rev);
    }
    return write_bb_black_respond(fileName,rev);
}

int write_bb_black_respond(char* fileName,char *rev)
{
    long len;
    char sw[15];

    char uid[60];
    char ip_s[20];
    char port_s[10];
    char port_l[10];
    char* pContent;
    //First read the old config
    FILE* fp = fopen(fileName, "rb");
    if(fp==NULL)
    {
     DEBUG_printf("open error\n");
     exit(1);
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
    cJSON *jsonroot = cJSON_Parse(pContent); //json
    strcpy(sw,cJSON_GetObjectItem(jsonroot,"api-switch")->valuestring);
    strcpy(uid,cJSON_GetObjectItem(jsonroot,"api-uid")->valuestring);

    strcpy(ip_s,cJSON_GetObjectItem(jsonroot,"ip_server")->valuestring);
    strcpy(port_s,cJSON_GetObjectItem(jsonroot,"port_server")->valuestring);
    strcpy(port_l,cJSON_GetObjectItem(jsonroot,"port_local")->valuestring);
    cJSON_Delete(jsonroot);
    //Write configuration, update
    fp = fopen(fileName, "w+");
    if(fp==NULL)
    {
     DEBUG_printf("open error\n");
    }
    cJSON *root;
    char *out;
    root=cJSON_CreateObject();
    cJSON_AddStringToObject(root, "api-switch" , sw);
    cJSON_AddStringToObject(root, "api-uid", uid);
    cJSON_AddStringToObject(root, "auth", rev);
    cJSON_AddStringToObject(root, "ip_server" ,ip_s);
    cJSON_AddStringToObject(root, "port_server", port_s);
    cJSON_AddStringToObject(root, "port_local", port_l);
    out=cJSON_Print(root);
    fprintf(fp,"%s",out);
    fclose(fp);

    cJSON_Delete(root);
    free(out);
    out = NULL;
    root = NULL;
    free(pContent);
    return 1;
}


int turnoff_bb_black(char* fileName)
{
    long len;
    char auth[15];
    char uid[60];
    char ip_s[20];
    char port_s[10];
    char port_l[10];
    char* pContent;
    //Read the old config firstly
    FILE* fp = fopen(fileName, "rb");
    if(fp==NULL)
    {
     DEBUG_printf("open error\n");
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
    cJSON *jsonroot = cJSON_Parse(pContent); //json
    strcpy(auth,cJSON_GetObjectItem(jsonroot,"auth")->valuestring);
    strcpy(uid,cJSON_GetObjectItem(jsonroot,"api-uid")->valuestring);

    strcpy(ip_s,cJSON_GetObjectItem(jsonroot,"ip_server")->valuestring);
    strcpy(port_s,cJSON_GetObjectItem(jsonroot,"port_server")->valuestring);
    strcpy(port_l,cJSON_GetObjectItem(jsonroot,"port_local")->valuestring);
    cJSON_Delete(jsonroot);
    //Write configuration, update
    fp = fopen(fileName, "w+");
    if(fp==NULL)
    {
     DEBUG_printf("open error\n");
    }
    cJSON *root;
    char *out;
    root=cJSON_CreateObject();
    cJSON_AddStringToObject(root, "api-switch" , "off");
    cJSON_AddStringToObject(root, "api-uid", uid);
    cJSON_AddStringToObject(root, "auth", auth);
    cJSON_AddStringToObject(root, "ip_server" ,ip_s);
    cJSON_AddStringToObject(root, "port_server", port_s);
    cJSON_AddStringToObject(root, "port_local", port_l);
    out=cJSON_Print(root);
    fprintf(fp,"%s",out);
    fclose(fp);

    cJSON_Delete(root);
    free(out);
    out = NULL;
    root = NULL;
    free(pContent);
    return 1;
}


POOLS_D_COUNT *rev_pools(char *buf_tmp ,POOLS_D_COUNT *get_pool_data)
{
    char *nextobj, *item, *nextitem;
    char *body_tmp;
	int count=0;
	int num=0;
    body_tmp=strchr(buf_tmp, ';');
    *(body_tmp++) = '\0';
    strcpy(rev_ID,buf_tmp);  //Get an ID
    buf_tmp=body_tmp;

	while (buf_tmp != NULL) {
		nextobj = strchr(buf_tmp, ';'); //;Symbols to cut of
		if (nextobj != NULL)
			*(nextobj++) = '\0';
		if (*buf_tmp) {
			item = buf_tmp;

			while (item != NULL) {
				nextitem = strchr(item, ','); // ,Symbols to cut of
				if (nextitem != NULL)
					*(nextitem++) = '\0';
                if (*item) {
                switch(count)
                {
                    case 0:
                     strcpy(get_pool_data->pools_data[num].which_pool,item);
                     get_pool_data->pools_count++;
                     count++;
                      break;
                    case 1:
                     strcpy(get_pool_data->pools_data[num].url,item);count++;
                      break;
                    case 2:
                     strcpy(get_pool_data->pools_data[num].user,item);count++;
                      break;
                    case 3:
                     strcpy(get_pool_data->pools_data[num].pass,item);count=0;
                      break;
                    default: printf("Pools Config Data is Wrong\n");
                      break;
                }
            }
				item = nextitem;
			}num++;
		}
		buf_tmp = nextobj;
	}
  return get_pool_data;
}



GETS4_CONFIG *save_old_conf(char* fileName,GETS4_CONFIG *get_save_conf)
{
    long len;
    char* pContent;
    int i=0;
    FILE* fp = fopen(fileName, "rb+");
    if(fp==NULL)
    {
     printf("open error\n");
    }
    fseek(fp,0,SEEK_END);
    len=ftell(fp);
    if(0 == len)
    {
        return NULL;
    }
    fseek(fp,0,SEEK_SET);
    pContent = (char*) malloc(sizeof(char)*len+100);
    fread(pContent,1,len,fp);
    fclose(fp);

    cJSON *jsonroot = cJSON_Parse(pContent); //Parse
    cJSON *taskArry=cJSON_GetObjectItem(jsonroot,"pools");//Take an array
    cJSON_GetArraySize(taskArry);//the size of the array
    cJSON *tasklist=taskArry->child;//The number of components

    while(tasklist!=NULL)
    {
       //printf("%s\n",cJSON_GetObjectItem(tasklist,"url")->valuestring);
        strcpy(get_save_conf->pools_data[i].url,cJSON_GetObjectItem(tasklist,"url")->valuestring);
        strcpy(get_save_conf->pools_data[i].user,cJSON_GetObjectItem(tasklist,"user")->valuestring);
        strcpy(get_save_conf->pools_data[i].pass,cJSON_GetObjectItem(tasklist,"pass")->valuestring);
        i++;
        tasklist=tasklist->next;
     }

     //strcpy(get_save_conf->api_listen,cJSON_GetObjectItem(jsonroot,"api-listen")->valueint);
     //strcpy(get_save_conf->api_network,cJSON_GetObjectItem(jsonroot,"api-network")->valueint);
	 get_save_conf->api_listen=cJSON_GetObjectItem(jsonroot,"api-listen")->valueint;
     get_save_conf->api_network=cJSON_GetObjectItem(jsonroot,"api-network")->valueint;
     get_save_conf->bitmain_nobeeper=cJSON_GetObjectItem(jsonroot,"bitmain-nobeeper")->valueint;
     get_save_conf->bitmain_notempoverctrl=cJSON_GetObjectItem(jsonroot,"bitmain-notempoverctrl")->valueint;

     strcpy(get_save_conf->api_allow,cJSON_GetObjectItem(jsonroot,"api-allow")->valuestring);
     strcpy(get_save_conf->bitmain_freq,cJSON_GetObjectItem(jsonroot,"bitmain-freq")->valuestring);
     strcpy(get_save_conf->bitmain_voltage,cJSON_GetObjectItem(jsonroot,"bitmain-voltage")->valuestring);
    //*/
    free(pContent);
    cJSON_Delete(jsonroot);
    return get_save_conf;
}

void create_conf(GETS4_CONFIG *new_pools,char* fileName)
{
    cJSON *root,*fld,*array;
    char *out;
    FILE* fp = fopen(fileName, "w+");
     if(fp==NULL)
    {
     printf("open error\n");
    }

    root=cJSON_CreateObject();
	cJSON_AddItemToObject(root, "pools", array=cJSON_CreateArray());;
    int i;
    for(i=0;i<3;i++)
    {
    cJSON_AddItemToArray(array,fld=cJSON_CreateObject());
    cJSON_AddStringToObject(fld, "url", new_pools->pools_data[i].url);
    cJSON_AddStringToObject(fld, "user", new_pools->pools_data[i].user);
    cJSON_AddStringToObject(fld, "pass", new_pools->pools_data[i].pass);
    }
    //cJSON_AddStringToObject(root, "api-listen", new_pools->api_listen);
    //cJSON_AddStringToObject(root, "api-network",new_pools->api_network);
    if(1==new_pools->api_listen)
     cJSON_AddTrueToObject(root, "api-listen");
    else if(0==new_pools->api_listen)
     cJSON_AddFalseToObject(root, "api-listen");

    if(1==new_pools->api_network)
     cJSON_AddTrueToObject(root, "api-network");
    else if(0==new_pools->api_network)
     cJSON_AddFalseToObject(root, "api-network");

    if(1==new_pools->bitmain_nobeeper)
     cJSON_AddTrueToObject(root, "bitmain-nobeeper");
	if(1==new_pools->bitmain_notempoverctrl)
     cJSON_AddTrueToObject(root, "bitmain-notempoverctrl");

    cJSON_AddStringToObject(root, "api-allow" , new_pools->api_allow);
    cJSON_AddStringToObject(root, "bitmain-freq", new_pools->bitmain_freq);
    cJSON_AddStringToObject(root, "bitmain-voltage", new_pools->bitmain_voltage);

    out=cJSON_Print(root);
    fprintf(fp,"%s",out);
    fclose(fp);

    cJSON_Delete(root);
    free(out);
    out = NULL;
    root = NULL;

}

GETS4_CONFIG *creat_newpools(POOLS_D_COUNT *change,GETS4_CONFIG *fresh_old)
{
    printf("change->pools_count = %d\n",change->pools_count);
    if(change->pools_count==1) //There is a pool changes
    {
        int which_p;
        which_p=atoi(change->pools_data[0].which_pool);//which pool change
        switch(which_p)
        {
            case 0:
                strcpy(fresh_old->pools_data[0].url ,change->pools_data[0].url);
                strcpy(fresh_old->pools_data[0].user,change->pools_data[0].user);
                strcpy(fresh_old->pools_data[0].pass,change->pools_data[0].pass);
                break;
            case 1:
                strcpy(fresh_old->pools_data[1].url ,change->pools_data[0].url);
                strcpy(fresh_old->pools_data[1].user ,change->pools_data[0].user);
                strcpy(fresh_old->pools_data[1].pass ,change->pools_data[0].pass);
                break;
            case 2:
                strcpy(fresh_old->pools_data[2].url ,change->pools_data[0].url);
                strcpy(fresh_old->pools_data[2].user ,change->pools_data[0].user);
                strcpy(fresh_old->pools_data[2].pass ,change->pools_data[0].pass);
                break;
        }
    }
    else if(change->pools_count==2)
    {
        int which_p1,which_p2;
        which_p1=atoi(change->pools_data[0].which_pool);//whick change
        which_p2=atoi(change->pools_data[1].which_pool);//which change
        switch(which_p1)
        {
            case 0:
                strcpy(fresh_old->pools_data[0].url ,change->pools_data[0].url);
                strcpy(fresh_old->pools_data[0].user ,change->pools_data[0].user);
                strcpy(fresh_old->pools_data[0].pass ,change->pools_data[0].pass);
                break;
            case 1:
                strcpy(fresh_old->pools_data[1].url ,change->pools_data[0].url);
                strcpy(fresh_old->pools_data[1].user ,change->pools_data[0].user);
                strcpy(fresh_old->pools_data[1].pass ,change->pools_data[0].pass);
                break;
            case 2:
                strcpy(fresh_old->pools_data[2].url ,change->pools_data[0].url);
                strcpy(fresh_old->pools_data[2].user ,change->pools_data[0].user);
                strcpy(fresh_old->pools_data[2].pass ,change->pools_data[0].pass);
                break;
        }
        switch(which_p2)
        {
            case 0:
                strcpy(fresh_old->pools_data[0].url ,change->pools_data[1].url);
                strcpy(fresh_old->pools_data[0].user ,change->pools_data[1].user);
                strcpy(fresh_old->pools_data[0].pass ,change->pools_data[1].pass);
                break;
            case 1:
                strcpy(fresh_old->pools_data[1].url ,change->pools_data[1].url);
                strcpy(fresh_old->pools_data[1].user ,change->pools_data[1].user);
                strcpy(fresh_old->pools_data[1].pass ,change->pools_data[1].pass);
                break;
            case 2:
                strcpy(fresh_old->pools_data[2].url ,change->pools_data[1].url);
                strcpy(fresh_old->pools_data[2].user ,change->pools_data[1].user);
                strcpy(fresh_old->pools_data[2].pass ,change->pools_data[1].pass);
                break;
        }
    }
    else if(change->pools_count==3)
    {
        strcpy(fresh_old->pools_data[0].url ,change->pools_data[0].url);
        strcpy(fresh_old->pools_data[0].user,change->pools_data[0].user);
        strcpy(fresh_old->pools_data[0].pass ,change->pools_data[0].pass);
        strcpy(fresh_old->pools_data[1].url ,change->pools_data[1].url);
        strcpy(fresh_old->pools_data[1].user ,change->pools_data[1].user);
        strcpy(fresh_old->pools_data[1].pass ,change->pools_data[1].pass);
        strcpy(fresh_old->pools_data[2].url ,change->pools_data[2].url);
        strcpy(fresh_old->pools_data[2].user ,change->pools_data[2].user);
        strcpy(fresh_old->pools_data[2].pass ,change->pools_data[2].pass);
    }
    return fresh_old;

}


////////////////////////////////////////////////////////////////////////
//////////////////////////S3 miner//////////////////////////////
GETS3_CONFIG *save_openwrt_webconf(GETS3_CONFIG *save_config)
{
    #ifdef S3_single_quotes
    strcpy(save_config->pools_data[0].url,read_cgminer_conf("'pool1url'"));
    strcpy(save_config->pools_data[0].user,read_cgminer_conf("'pool1user'"));
    strcpy(save_config->pools_data[0].pass,read_cgminer_conf("'pool1pw'"));
    strcpy(save_config->pools_data[1].url,read_cgminer_conf("'pool2url'"));
    strcpy(save_config->pools_data[1].user,read_cgminer_conf("'pool2user'"));
    strcpy(save_config->pools_data[1].pass,read_cgminer_conf("'pool2pw'"));
    strcpy(save_config->pools_data[2].url,read_cgminer_conf("'pool3url'"));
    strcpy(save_config->pools_data[2].user,read_cgminer_conf("'pool3user'"));
    strcpy(save_config->pools_data[2].pass,read_cgminer_conf("'pool3pw'"));

    strcpy(save_config->api_allow,read_cgminer_conf("'api_allow'"));
    strcpy(save_config->more_options,read_cgminer_conf("'more_options'"));
    strcpy(save_config->freq,read_cgminer_conf("'freq'"));
    strcpy(save_config->voltage,read_cgminer_conf("'voltage'"));
    strcpy(save_config->pool_balance,read_cgminer_conf("'pool_balance'"));
    strcpy(save_config->bitmain_nobeeper,read_cgminer_conf("'bitmain_nobeeper'"));
    strcpy(save_config->bitmain_notempoverctrl,read_cgminer_conf("'bitmain_notempoverctrl'"));
    #else
    strcpy(save_config->pools_data[0].url,read_cgminer_conf("pool1url"));
    strcpy(save_config->pools_data[0].user,read_cgminer_conf("pool1user"));
    strcpy(save_config->pools_data[0].pass,read_cgminer_conf("pool1pw"));
    strcpy(save_config->pools_data[1].url,read_cgminer_conf("pool2url"));
    strcpy(save_config->pools_data[1].user,read_cgminer_conf("pool2user"));
    strcpy(save_config->pools_data[1].pass,read_cgminer_conf("pool2pw"));
    strcpy(save_config->pools_data[2].url,read_cgminer_conf("pool3url"));
    strcpy(save_config->pools_data[2].user,read_cgminer_conf("pool3user"));
    strcpy(save_config->pools_data[2].pass,read_cgminer_conf("pool3pw"));

    strcpy(save_config->api_allow,read_cgminer_conf("api_allow"));
    strcpy(save_config->more_options,read_cgminer_conf("more_options"));
    strcpy(save_config->freq,read_cgminer_conf("freq"));
    strcpy(save_config->voltage,read_cgminer_conf("voltage"));
    strcpy(save_config->pool_balance,read_cgminer_conf("pool_balance"));
    strcpy(save_config->bitmain_nobeeper,read_cgminer_conf("bitmain_nobeeper"));
    strcpy(save_config->bitmain_notempoverctrl,read_cgminer_conf("bitmain_notempoverctrl"));
    #endif

    return save_config;
}
GETS3_CONFIG *creat_openwrt_newpools(POOLS_D_COUNT *openwrt_change,GETS3_CONFIG *refresh_openwrt)
{
    int which_p1,which_p2;
    if(openwrt_change->pools_count==1)
    {
        which_p1=atoi(openwrt_change->pools_data[0].which_pool);//which change
        strcpy(refresh_openwrt->pools_data[which_p1].url ,openwrt_change->pools_data[0].url);
        strcpy(refresh_openwrt->pools_data[which_p1].user ,openwrt_change->pools_data[0].user);
        strcpy(refresh_openwrt->pools_data[which_p1].pass ,openwrt_change->pools_data[0].pass);
    }
    else if(openwrt_change->pools_count==2)
    {
        which_p1=atoi(openwrt_change->pools_data[0].which_pool);//which change
        which_p2=atoi(openwrt_change->pools_data[1].which_pool);//which change
        strcpy(refresh_openwrt->pools_data[which_p1].url ,openwrt_change->pools_data[0].url);
        strcpy(refresh_openwrt->pools_data[which_p1].user ,openwrt_change->pools_data[0].user);
        strcpy(refresh_openwrt->pools_data[which_p1].pass ,openwrt_change->pools_data[0].pass);

        strcpy(refresh_openwrt->pools_data[which_p2].url ,openwrt_change->pools_data[1].url);
        strcpy(refresh_openwrt->pools_data[which_p2].user ,openwrt_change->pools_data[1].user);
        strcpy(refresh_openwrt->pools_data[which_p2].pass ,openwrt_change->pools_data[1].pass);
    }
    else if(openwrt_change->pools_count==3)
    {
        strcpy(refresh_openwrt->pools_data[0].url ,openwrt_change->pools_data[0].url);
        strcpy(refresh_openwrt->pools_data[0].user ,openwrt_change->pools_data[0].user);
        strcpy(refresh_openwrt->pools_data[0].pass ,openwrt_change->pools_data[0].pass);

        strcpy(refresh_openwrt->pools_data[1].url ,openwrt_change->pools_data[1].url);
        strcpy(refresh_openwrt->pools_data[1].user ,openwrt_change->pools_data[1].user);
        strcpy(refresh_openwrt->pools_data[1].pass ,openwrt_change->pools_data[1].pass);
        strcpy(refresh_openwrt->pools_data[2].url ,openwrt_change->pools_data[2].url);
        strcpy(refresh_openwrt->pools_data[2].user ,openwrt_change->pools_data[2].user);
        strcpy(refresh_openwrt->pools_data[2].pass ,openwrt_change->pools_data[2].pass);
    }
    return refresh_openwrt;

}
void write_openwrt_conf(GETS3_CONFIG *newdata)
{
    FILE* fp = fopen(luci_cgminer_dir, "rb+");
    if(fp==NULL)
    {
        printf("open error\n");
    }
    #ifdef S3_single_quotes
    fprintf(fp,"config cgminer 'default'\n");
    fprintf(fp,"  option 'pool1url'  '%s'\n",newdata->pools_data[0].url);
    fprintf(fp,"	option 'pool1user' '%s'\n",newdata->pools_data[0].user);
    fprintf(fp,"	option 'pool1pw'   '%s'\n",newdata->pools_data[0].pass);
    fprintf(fp,"	option 'pool2url'  '%s'\n",newdata->pools_data[1].url);
    fprintf(fp,"	option 'pool2user' '%s'\n",newdata->pools_data[1].user);
    fprintf(fp,"	option 'pool2pw'   '%s'\n",newdata->pools_data[1].pass);
    fprintf(fp,"	option 'pool3url'  '%s'\n",newdata->pools_data[2].url);
    fprintf(fp,"	option 'pool3user' '%s'\n",newdata->pools_data[2].user);
    fprintf(fp,"	option 'pool3pw'   '%s'\n",newdata->pools_data[2].pass);

    fprintf(fp,"	option 'api_allow'  '%s'\n",newdata->api_allow);
    fprintf(fp,"	option 'more_options' '%s'\n",newdata->more_options);
    fprintf(fp,"	option 'freq'   '%s'\n",newdata->freq);
    fprintf(fp,"	option 'voltage'   '%s'\n",newdata->voltage);
    fprintf(fp,"	option 'pool_balance' '%s'\n",newdata->pool_balance);
    fprintf(fp,"	option 'bitmain_nobeeper'   '%s'\n",newdata->bitmain_nobeeper);
    fprintf(fp,"	option 'bitmain_notempoverctrl' '%s'\n",newdata->bitmain_notempoverctrl);
    fprintf(fp,"	           ");
    #else
    fprintf(fp,"config cgminer 'default'\n");
    fprintf(fp,"  option pool1url  '%s'\n",newdata->pools_data[0].url);
    fprintf(fp,"	option pool1user '%s'\n",newdata->pools_data[0].user);
    fprintf(fp,"	option pool1pw   '%s'\n",newdata->pools_data[0].pass);
    fprintf(fp,"	option pool2url  '%s'\n",newdata->pools_data[1].url);
    fprintf(fp,"	option pool2user '%s'\n",newdata->pools_data[1].user);
    fprintf(fp,"	option pool2pw   '%s'\n",newdata->pools_data[1].pass);
    fprintf(fp,"	option pool3url  '%s'\n",newdata->pools_data[2].url);
    fprintf(fp,"	option pool3user '%s'\n",newdata->pools_data[2].user);
    fprintf(fp,"	option pool3pw   '%s'\n",newdata->pools_data[2].pass);

    fprintf(fp,"	option api_allow  '%s'\n",newdata->api_allow);
    fprintf(fp,"	option more_options '%s'\n",newdata->more_options);
    fprintf(fp,"	option freq   '%s'\n",newdata->freq);
    fprintf(fp,"	option voltage   '%s'\n",newdata->voltage);
    fprintf(fp,"	option pool_balance '%s'\n",newdata->pool_balance);
    fprintf(fp,"	option bitmain_nobeeper   '%s'\n",newdata->bitmain_nobeeper);
    fprintf(fp,"	option bitmain_notempoverctrl '%s'\n",newdata->bitmain_notempoverctrl);
    fprintf(fp,"	           ");
    #endif

    fclose(fp);
}

//write s3 web auth flag ,f6 is error
int write_openwrt_auth(char *res)
{
    FILE*fp=NULL;
    fp=fopen("/etc/config/auth_tmp.txt","w+");
    if(NULL==fp)
    {
    return-1; // open creat error
    }
  if(strcmp(res,"f5")==0)
    fprintf(fp,"%s",res);
  else
	fprintf(fp,"%s","fx");
    fclose(fp);
    fp=NULL; //clear
    return 1;
}

#if 0
#endif
