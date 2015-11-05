#!/bin/sh

echo "starting test"
/etc/init.d/cgminer.sh stop
/etc/init.d/initc.sh restart
echo "  Starting test" >  /tmp/test_result.data

if [ -z  "`lsmod | grep bitmain_lcd`"  ]; then
	echo "No bitmain-lcd"
	insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_lcd.ko SLEEP_TIME=3 testing_hash=1
else
	echo "Have bitmain-lcd"
	rmmod bitmain_lcd.ko
	sleep 1
	insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_lcd.ko SLEEP_TIME=3 testing_hash=1
fi

if [ -z  "`lsmod | grep bitmain_spi`"  ]; then
	echo "No bitmain-asic"
	insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_spi.ko
else
 	echo "Have bitmain-asic"
	rmmod bitmain_spi.ko
	sleep 1
	insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_spi.ko fpga_ret_prnt=0 rx_st_prnt=0
fi
#beep gpio0_20
beep=20
if [ ! -e /sys/class/gpio/gpio$beep ]; then
	echo $beep > /sys/class/gpio/export
fi
echo low > /sys/class/gpio/gpio$beep/direction
echo 1 > /sys/class/gpio/gpio$beep/value
usleep $((200*1000))
echo 0 > /sys/class/gpio/gpio$beep/value
sleep 5
echo " Test Result" >  /tmp/test_result.data
/usr/bin/test-hash
