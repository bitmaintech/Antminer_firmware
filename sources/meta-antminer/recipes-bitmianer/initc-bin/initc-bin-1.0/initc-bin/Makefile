CFLAGS=-g
exe = initc

all: $(exe)

clean: clean-$(exe)-debug
clean-$(exe)-debug:
	rm -f $(exe)-debug

$(exe)-debug: $(exe).c
	$(CROSS_COMPILE)gcc $(CFLAGS) $(exe).c -g -o $(exe)-debug

clean: clean-$(exe)
clean-$(exe):
	rm -f $(exe)

$(exe): $(exe)-debug
	$(CROSS_COMPILE)strip $(exe)-debug -o $(exe)

