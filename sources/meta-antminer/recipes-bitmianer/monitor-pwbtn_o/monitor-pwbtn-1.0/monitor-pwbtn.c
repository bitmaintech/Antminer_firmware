#include <linux/input.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void green_on(void)
{
	system("echo low >/sys/class/gpio/gpio71/direction");
}

void green_off(void)
{
	system("echo high >/sys/class/gpio/gpio71/direction");
}

void do_poweroff(int sig)
{
	system("poweroff");
}

int main(int argc, char **argv)
{
	struct timeval last = {0,0};
	int count = 0;
	int state = 0;
	int fd = open("/dev/input/event0", O_RDONLY);
	if(fd < 0)
	  return fd;
	
	struct input_event ev;
	while (read(fd, &ev, sizeof(ev)) > 0) {
		if (ev.type == EV_KEY && ev.code == KEY_POWER && ev.value == 1) {
			struct timeval diff;
			timersub(&ev.time, &last, &diff);
			if (diff.tv_sec > 10) {
				signal(SIGALRM, do_poweroff);
				alarm(5);
			} else {
				alarm(0);
			}
			if (state == 0 && diff.tv_sec == 0) {
				if (!count)
					count+=1;
				if (++count >= 5) {
					green_on();
					state = 1;
					count = 0;
				}
			}
			else
			if (state == 1 && diff.tv_sec >= 5 && diff.tv_sec <= 10) {
				state = 2;
				count = 1;
			}
			else
			if (state == 2 && diff.tv_sec == 0) {
				if (++count >= 5) {
					pid_t pid = fork();
					if (pid == 0) {
						dup2(1, 0);
						execvp(argv[1], argv+1);
						exit(1);
					}
					if (pid > 0)
						wait(NULL);
					state = 0;
					count = 0;
				}
			}
			else {
				green_off();
				state = 0;
				count = 0;
			}
			last = ev.time;
		}
	}
}
