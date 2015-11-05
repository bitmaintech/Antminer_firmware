#include <linux/input.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef enum{false=0,true=!false} bool ;

//GPIO 2_2
//GPIO2_5 recover_key GPIO2_3
//Rev 001
/*
# gpio0_23 = 23 = red LED
# gpio1_13 = 45 = green LED
# gpio1_14  = 46 = recovery key
*/
void rg_on(void)
{
	//system("echo low >/sys/class/gpio/gpio66/direction");
	//system("echo low >/sys/class/gpio/gpio69/direction");
	system("echo low >/sys/class/gpio/gpio23/direction");
	system("echo low >/sys/class/gpio/gpio45/direction");
}

void rg_off(void)
{
	//system("echo high >/sys/class/gpio/gpio66/direction");
	//system("echo high >/sys/class/gpio/gpio69/direction");
	system("echo high >/sys/class/gpio/gpio23/direction");
	system("echo high >/sys/class/gpio/gpio45/direction");
}

bool get_gpio(FILE *pFile)
{
	unsigned int value;
	FILE *pRreco = NULL;
	//if((pRreco = fopen("/sys/class/gpio/gpio67/value", "r")) == NULL)
	if((pRreco = fopen("/sys/class/gpio/gpio46/value", "r")) == NULL)
	{
	    printf("Open read recovery button failure\n");
		return -1;
	}
	//fscanf(fp,"%d%s", &i, s);
	fscanf(pRreco,"%d",&value);
	//printf("value = %d\n", value);
	close(pRreco);
	if(value)
		return true;
	else
		return false;
}

int main(int argc, char **argv)
{
	int count = 0;
	int state = 0;
	FILE *pRreco = NULL;
	struct timeval start,end;
	printf("monitor-recobtn compile %s--%s\n", __DATE__,__TIME__);
	while(1)
	{
		if(get_gpio(pRreco) == false)
		{
			gettimeofday( &start, NULL );
			rg_on();
			printf("Detect recovery button push off\n");
			while(1)
			{
				if(get_gpio(pRreco) == true)
				{
					rg_off();
					break;
				}
				gettimeofday( &end, NULL );
				//3S
				if((1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec) > (1000000 * 3))
				{
					printf("recovery button off over 3S, wait push on\n");
					while(get_gpio(pRreco) == false)
					{
						system("echo high >/sys/class/gpio/gpio20/direction");
						rg_on();
						sleep(1);
						rg_off();
						sleep(1);
					}
					system("echo low >/sys/class/gpio/gpio20/direction");
					printf("recovery button on\n");
					pid_t pid = fork();
					if ( pid < 0) /* Èç¹û³ö´í */
						printf("error occured.\n");
					else if (pid == 0) {
						rg_off();
						//dup2(1, 0);
						if(argc >= 2)
							execvp(argv[1], argv+1);
						else
						{
							sleep(1);
						}
						exit(0);
					}
					else if (pid > 0)
					{
						wait(NULL);
						printf("child complete\n");
						for(count = 0; count < 100; count++)
						{
							rg_on();
							usleep(100*1000);
							rg_off();
							usleep(100*1000);
						}
						printf("recovery factory complete\n");
					}
				}
				else
					usleep(500*1000);
			}
		}
		else
			sleep(1);
	}
}




