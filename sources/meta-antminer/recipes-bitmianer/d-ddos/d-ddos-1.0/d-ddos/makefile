OBJECTS=d-ddos

all:$(OBJECTS)

PKG_NAME=d-ddos

$(OBJECTS): aes.o cjson.o cgminer_api.o d_ddos.o ping.o get_ip.o construct_ip_struct.o
	$(CC) $(LDFLAGS) $^ -o $@ -lcurl -lcrypto -lm

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	@-rm *.o $(OBJECTS)

