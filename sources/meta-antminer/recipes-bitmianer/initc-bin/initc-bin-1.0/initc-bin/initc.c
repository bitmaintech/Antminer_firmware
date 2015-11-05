#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "time.h"

typedef enum{false=0,true=!false} bool ;

/***************************
DCLK	 gpio2_6  gpio70	P8 45
nConfig	gpio2_8	gpio72	P8 43 in
nStatus	gpio2_10		gpio74	P8 41
CONF_DONE	gpio2_12		gpio76	P8 39 in
Data		gpio3_15		gpio111	P9 29 out
***************************/

void ctrl_gpio(FILE *pFile, bool value)
{
	if(value !=0 )
		value = 1;
	fprintf(pFile,"%d",value);
	fflush(pFile);
	return;
}
bool get_gpio(FILE *pFile)
{
	unsigned int value;
	//fscanf(fp,"%d%s", &i, s);
	fscanf(pFile,"%d",&value);
	if(value)
		return true;
	else
		return false;
}
static unsigned char fdata_buffer[1024*8];
typedef struct __FPGA_DATA
{
	unsigned int data_len;
	unsigned char *pdata;
	unsigned int nStatus;

}FPGA_DATA;

#define FPGA_DL_IOC_MAGIC 	'p'

#define START_CONFIG	_IOWR(FPGA_DL_IOC_MAGIC, 0, unsigned int)
#define CONFIG_DATA		_IOWR(FPGA_DL_IOC_MAGIC, 1, FPGA_DATA)
#define CONFIG_DONE		_IOWR(FPGA_DL_IOC_MAGIC, 2, unsigned int)

#if 0
int main(void)
{
	FILE *pDclk = NULL, *pCDone = NULL, *pnConf = NULL, *pnSt = NULL, *pData = NULL;
	FILE *pRbf = NULL;
	FILE *p = NULL;
	int ret = 222;
	unsigned int i = 0, j = 0;
	unsigned char fpga_data;
	int rc = 0;
	time_t t; //Ê±¼ä½á¹¹»òÕß¶ÔÏó
	//t=time(NULL); //»ñÈ¡µ±Ç°ÏµÍ³µÄÈÕÀúÊ±¼ä

	if((pRbf = fopen("spitop_noncerev.rbf", "rb")) == NULL)
	{
		printf("open rbf err\n");
		goto Rbf_err;
	}
	if((pDclk = fopen("/sys/class/gpio/gpio70/value", "w")) == NULL)
	{
		printf("open Dclk value err\n");
		goto Dclk_err;
	}
	if((pCDone = fopen("/sys/class/gpio/gpio72/value", "r")) == NULL)
	{
		printf("open ConfigDone value err\n");
		goto CDOne_err;
	}
	if((pnConf = fopen("/sys/class/gpio/gpio74/value", "w")) == NULL)
	{
		printf("open nConfig value err\n");
		goto nConf_err;
	}
	if((pnSt = fopen("/sys/class/gpio/gpio76/value", "r")) == NULL)
	{
		printf("open nStatus value err\n");
		goto nSt_err;
	}

	p = fopen("/sys/class/gpio/export","w");
	fprintf(p,"%d",111);
	fclose(p);
	p = fopen("/sys/class/gpio/gpio111/direction","w");
	fprintf(p,"out");
	fclose(p);

	if((pData= fopen("/sys/class/gpio/gpio111/value", "w")) == NULL)
	{
		printf("open Data value err\n");
		goto Data_err;
	}
	t=time(NULL);
	printf("%s:start ps config FPGA\n",ctime(&t));
	ctrl_gpio(pnConf, false);// nCONFIG="0"£¬Ê¹FPGA½øÈëÅäÖÃ×´Ì¬
	ctrl_gpio(pDclk, false);
	usleep(5);
	// ¼ì²ânSTATUS£¬Èç¹ûÎª"0"£¬±íÃ÷FPGAÒÑÏìÓ¦ÅäÖÃÒªÇó£¬¿É¿ªÊ¼½øÐÐÅäÖÃ¡£·ñÔò±¨´í
	if (get_gpio(pnSt)!= 0)
	{
		printf("FPGA don't responed config\n");
		goto Data_err;
	}
	ctrl_gpio(pnConf, true);
	t=time(NULL);
	// ¿ªÊ¼Êä³öÅäÖÃÊý¾Ý£
	printf("Start download data--time %s\n", ctime(&t));
	while((rc = fread(fdata_buffer, 1, sizeof(fdata_buffer), pRbf)) != 0 )
	{
		//printf("read rbf len=%d\n", rc);
		for(j = 0; j < rc; j++)
		{
			fpga_data = fdata_buffer[j];
			//printf("fpga_data %#x\n", fpga_data);
			for (i=0; i<8; i++)
			{ // DCLK="0"Ê±£¬ÔÚData0ÉÏ·ÅÖÃÊý¾Ý£¨LSB first£©
				ctrl_gpio(pData, fpga_data&0x01);
				ctrl_gpio(pDclk, true);// DCLK->"1"£¬Ê¹FPGA¶ÁÈëÊý¾Ý
				fpga_data >>= 1; // ×¼±¸ÏÂÒ»Î»Êý¾Ý
				if (get_gpio(pnSt) == 0)
				{ // ¼ì²ânSTATUS£¬Èç¹ûÎª"0"£¬±íÃ÷FPGAÅäÖÃ³ö´í
					printf("FPGA config err\n");
					goto Data_err;
				}
				ctrl_gpio(pDclk, false);
			}
		}
	}
	// FPGA³õÊ¼»¯£º
	// ACEX 1KºÍFLEX 10KEÐèÒª10¸öÖÜÆÚ£¬APEX 20KÐèÒª40¸öÖÜÆÚ
	for(i=0; i<10; i++)
	{
		ctrl_gpio(pDclk, true);
		usleep(10);
		ctrl_gpio(pDclk, false);
		usleep(10);
	}
	ctrl_gpio(pData, false);
	if (get_gpio(pnConf) == 0)
	{ // ¼ì²ânCONF_Done£¬Èç¹ûÎª"0"£¬±íÃ÷FPGAÅäÖÃÎ´³É¹¦
		printf("Configure failure\n");
		goto Data_err;
	}
	#if 0
	for(i=0;i<100;i++)
	{
		printf("set 1\n");
		fprintf(pDclk,"%d",1);
		fflush(pDclk);
		usleep(10*1000);
		//sleep(1);
		printf("clear 0\n");
		fprintf(pDclk,"%d",0);
		fflush(pDclk);
		usleep(10*1000);
		//sleep(1);
	}
	#endif
	ctrl_gpio(pData, true);
	p = fopen("/sys/class/gpio/unexport","w");
	fprintf(p,"%d",111);
	fclose(p);
	t=time(NULL);
	printf("FPGA configure OK -- %s\n", ctime(&t));
	ret = 0;
	Data_err:
		ctrl_gpio(pnConf, true);
		ctrl_gpio(pDclk, true);
		fclose(pnSt);
	nSt_err:
		fclose(pnConf);
	nConf_err:
		fclose(pCDone);
	CDOne_err:
		fclose(pDclk);
	Dclk_err:
		fclose(pRbf);
	Rbf_err:
		return ret;
}
#else
int main(void)
{
	FILE *pRbf = NULL;
	int fpga_dl_fd;
	int ret = 222;
	unsigned int nStatus = 0;
	int rc = 0;
	FPGA_DATA fpga_data_buff;
	time_t t; //Ê±¼ä½á¹¹»òÕß¶ÔÏó

	if((pRbf = fopen("spitop_noncerev.rbf", "rb")) == NULL)
	{
		printf("open rbf err\n");
		goto Rbf_err;
	}
	if((fpga_dl_fd = open("/dev/bitmain-asic", O_RDWR)) < 0)
	{
		printf("fpga_dl open error!!\n");
		goto Open_DL_err;
	}
	printf("fpga_dl_fd is %x\n",fpga_dl_fd);
	ioctl(fpga_dl_fd, START_CONFIG, &nStatus);
	if(nStatus == 0)
		goto Conf_err;
	t=time(NULL); //»ñÈ¡µ±Ç°ÏµÍ³µÄÈÕÀúÊ±¼ä
	printf("Start download data--time %s\n", ctime(&t));
	while((rc = fread(fdata_buffer, 1, sizeof(fdata_buffer), pRbf)) != 0 )
	{
		fpga_data_buff.data_len = rc;
		fpga_data_buff.pdata = fdata_buffer;
		ioctl(fpga_dl_fd, CONFIG_DATA, &fpga_data_buff);
		if(fpga_data_buff.nStatus == 0)
		{
			goto Conf_err;
		}
	}
	ioctl(fpga_dl_fd, CONFIG_DONE, &nStatus);
	t=time(NULL);
	if(fpga_data_buff.nStatus == 0)
	{
		printf("FPGA configure failure -- %s\n", ctime(&t));
		goto Conf_err;
	}
	printf("FPGA configure OK -- %s\n", ctime(&t));
	ret = 0;
	Conf_err:
		close(fpga_dl_fd);
	Open_DL_err:
		fclose(pRbf);
	Rbf_err:
		return ret;
}

#endif



