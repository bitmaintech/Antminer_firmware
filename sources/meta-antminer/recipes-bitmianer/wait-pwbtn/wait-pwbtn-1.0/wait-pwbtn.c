#include <linux/input.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
			break;
		}
	}
}
