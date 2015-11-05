#!/bin/sh

#set -x
ant_ip=
ant_ghs=
ant_fan=
ant_temp=
ant_poolurl=
ant_pooluser=

#if false; then
if true; then
if [ -z  "`lsmod | grep bitmain_lcd`"  ]; then
	echo "No bitmain-lcd"
	insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_lcd.ko
else
	echo "Have bitmain-lcd"
	rmmod bitmain_lcd.ko
	sleep 1
	insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_lcd.ko
fi
fi

if [ ! -f /config/user_defined_lcd ] ; then
	cp /etc/user_defined_lcd.factory /config/user_defined_lcd
fi

while true; do
    ant_tmp=`ifconfig | grep "inet addr"`
    i=0
    for ant_var in ${ant_tmp}
    do
    	case ${i} in
    		1 )
    		ant_ip=${ant_var}
    		ant_ip=${ant_ip/addr:/}
    		;;
    	esac
    	i=`expr $i + 1`
    done
    
    ant_tmp=`cgminer-api -o lcd`
    
    if [ "${ant_tmp}" == "Socket connect failed: Connection refused" ]; then
    	ant_ghs=0
    	ant_fan=0
    	ant_temp=0
    	ant_poolurl=no
    	ant_pooluser=no
    else
    	ant_ghs=${ant_tmp#*GHSavg=}
    	ant_ghs=${ant_ghs%%,fan=*}
    	
    	ant_fan=${ant_tmp#*fan=}
    	ant_fan=${ant_fan%%,temp=*}
    	
    	ant_temp=${ant_tmp#*temp=}
    	ant_temp=${ant_temp%%,pool=*}
    	
    	ant_poolurl=${ant_tmp#*pool=}
    	ant_poolurl=${ant_poolurl%%,user=*}
    	ant_var=${ant_poolurl:0:7}
		if [ "${ant_var}" == "http://" ]; then
			ant_poolurl=${ant_poolurl:7}
    	fi
		ant_pooluser=${ant_tmp#*user=}
    	ant_pooluser=${ant_pooluser%%|*}
    fi

   	if false; then 
    echo "IP:${ant_ip}"                 > /tmp/lcd.data
    echo "${ant_ghs}"              >> /tmp/lcd.data
    echo "${ant_fan} ${ant_temp}"  >> /tmp/lcd.data
    echo "Pl:${ant_poolurl}"         >> /tmp/lcd.data
    echo "Ur:${ant_pooluser}"        >> /tmp/lcd.data
	fi
	if [ ! -f /config/user_defined_lcd ] ; then
		echo "AntMiner S2"			 >  /tmp/lcd.data
	else
		echo "`cat /config/user_defined_lcd`"	 >  /tmp/lcd.data
	fi
	echo "${ant_ip}"             >> /tmp/lcd.data
	echo "${ant_ghs} GH/s"       >> /tmp/lcd.data
	#echo "${ant_temp}°C"		 >> /tmp/lcd.data
	echo "Max ${ant_temp}"			 >> /tmp/lcd.data

	sleep 10s
	done
