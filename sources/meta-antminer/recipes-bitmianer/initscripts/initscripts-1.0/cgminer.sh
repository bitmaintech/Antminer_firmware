#!/bin/sh

PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
DAEMON=/usr/bin/cgminer
NAME=cgminer
DESC="Cgminer daemon"
CONFIG_NAME="/config/asic-freq.config"
USER_SETTING="/config/user_setting"
set -e
#set -x
test -x "$DAEMON" || exit 0

do_start() {
	# gpio1_16 = 48 = net check LED
	if [ ! -e /sys/class/gpio/gpio48 ]; then
		echo 48 > /sys/class/gpio/export
	fi
	echo low > /sys/class/gpio/gpio48/direction

	gateway=$(route -n | grep 'UG[ \t]' | awk '{print $2}')
	if [ x"" == x"$gateway" ]; then
		gateway="192.168.1.1"
	fi	
	if [ "`ping -w 1 -c 1 $gateway | grep "100%" >/dev/null`" ]; then                                                   
		prs=1                                                
		echo "$gateway is Not reachable"                             
	else                                               
	    prs=0
		echo "$gateway is reachable" 	
	fi                    
	#ping $gateway -W1 -c1 & > /dev/null
	#prs=$?
	if [ $prs = "0" ]; then
		echo heartbeat > /sys/class/leds/beaglebone:green:usr3/trigger
		echo 1 > /sys/class/gpio/gpio48/value	
	else
		echo none > /sys/class/leds/beaglebone:green:usr3/trigger
		return
	fi
	sleep 5s
	if [ -z  "`lsmod | grep bitmain_spi`"  ]; then
		echo "No bitmain-asic"
		insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_spi.ko
	else
		echo "Have bitmain-asic"
		rmmod bitmain_spi.ko
		sleep 1
		insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_spi.ko fpga_ret_prnt=0 rx_st_prnt=0
		#insmod /mnt/mmc1/bitmain_spi.ko fpga_ret_prnt=0 rx_st_prnt=0
	fi
	#control console printk level
	#freq_value="`awk '{if($1 == "option" && $2=="\047freq_value\047") print $3}' $CONFIG_NAME | sed "s/'//g"`"
	#chip_value=`awk '{if($1 == "option" && $2=="\047chip_freq\047") print $3}' $CONFIG_NAME | sed "s/'//g"`
	#timeout=`awk '{if($1 == "option" && $2=="\047timeout\047") print $3}' $CONFIG_NAME | sed "s/'//g"`
	freq_value=0782
	chip_value=200
	chip_num=40
	freq_m=$(($chip_value * 1000))                                                                           
	timeout=$((2 ** (32 - 8) * (256 / $chip_num) / freq_m / 64))                                             
	echo $timeout

	queue_value="`awk '{if($1 == "queue") print $2}' $USER_SETTING | sed "s/'//g"`"
	echo " queue_vale=$queue_value"
	if [ -z $queue_value ]; then
		queue_value=8192
	fi

	#PARAMS="--bitmain-dev /dev/bitmain-asic --bitmain-options 115200:32:8:$timeout:$chip_value:$real_freq"
	PARAMS="--bitmain-dev /dev/bitmain-asic --bitmain-options 115200:32:8:$timeout:$chip_value:$freq_value:0725 --bitmain-checkn2diff --bitmain-hwerror --version-file /usr/bin/compile_time --queue $queue_value"
	echo PARAMS = $PARAMS
	start-stop-daemon -b -S -x screen -- -S cgminer -t cgminer -m -d "$DAEMON" $PARAMS --api-listen --default-config /config/cgminer.conf
	#cgminer $PARAMS -D --api-listen --default-config /config/cgminer.conf 2>&1 | tee log
}

do_stop() {
        killall -9 cgminer || true
}
case "$1" in
  start)
        echo -n "Starting $DESC: "
	do_start
        echo "$NAME."
        ;;
  stop)
        echo -n "Stopping $DESC: "
	do_stop
        echo "$NAME."
        ;;
  restart|force-reload)
        echo -n "Restarting $DESC: "
        do_stop
        do_start
        echo "$NAME."
        ;;
  *)
        N=/etc/init.d/$NAME
        echo "Usage: $N {start|stop|restart|force-reload}" >&2
        exit 1
        ;;
esac

exit 0
