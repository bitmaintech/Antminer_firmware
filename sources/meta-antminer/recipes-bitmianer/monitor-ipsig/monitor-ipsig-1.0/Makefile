CFLAGS=-g
exe = monitor-ipsig


all: $(exe)

$(exe): get_mac_ip.o monitor-ipsig.o 
	$(CC) $^ -o $@  

%.o : %.c
	$(CC)  -c $< -o $@

clean: clean-$(exe)
clean-$(exe):
	rm -f $(exe)

