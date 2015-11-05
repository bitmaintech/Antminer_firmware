#!/bin/sh

#version 001:
# gpio0_23 = 23 = red LED
# gpio1_13 = 45 = green LED
# gpio1_14  = 46 = recovery key
#version 000:
# gpio2_5 = 69 = red LED
# gpio2_2 = 66 = green LED

#set -x
ver=1
if [ "3"x = "$ver"x ]; then
    red_led=71
    green_led=72
elif [ "1"x = "$ver"x ]; then
    red_led=23
    green_led=45
else
    red_led=69
    green_led=66
fi

if [ ! -e /sys/class/gpio/gpio$red_led ]; then
	echo $red_led > /sys/class/gpio/export
fi
# gpio2_2 = 66 = green LED 
if [ ! -e /sys/class/gpio/gpio$green_led ]; then
	echo $green_led > /sys/class/gpio/export
fi

echo '#!/bin/sh' > /sbin/reboot.safe
#echo 'i2cset -y 1 0x24 0xb 0x6b' >> /sbin/reboot.safe
#echo 'i2cset -y 1 0x24 0x16 0' >> /sbin/reboot.safe
echo 'reboot -f' >> /sbin/reboot.safe
chmod a+x /sbin/reboot.safe

# Turn ON red LED, turn ON green LED
echo high > /sys/class/gpio/gpio$red_led/direction
echo high > /sys/class/gpio/gpio$green_led/direction

echo Starting initc
cd /usr/bin

exit_code=222
i=0
while [ $exit_code -eq 222 ] ; do
#       echo low > /sys/class/gpio/gpio49/direction # !pwr_en
#echo low > /sys/class/gpio/gpio76/direction # reset
 		echo "Config FPGA $i"
	    if [ -z  "`lsmod | grep bitmain_spi`"  ]; then
			echo "No bitmain-asic"
			insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_spi.ko
		else
			echo "Have bitmain-asic"
			rmmod bitmain_spi.ko
			sleep 1
			insmod /lib/modules/`uname -r`/kernel/drivers/bitmain/bitmain_spi.ko
		fi
# echo high > /sys/class/gpio/gpio76/direction # !reset
		./initc //FPGA download
        exit_code=$?
        i=$((i+1))
        if [[ $i -gt 5 ]] ; then
                break
        fi
done
if [[ $exit_code = 0 ]] ; then
        # Turn OFF red, Turn ON green
        echo high > /sys/class/gpio/gpio$green_led/direction
        echo low > /sys/class/gpio/gpio$red_led/direction

	#./waas -zd
else
        # Turn ON red, Turn OFF green
        echo low > /sys/class/gpio/gpio$green_led/direction
        echo high > /sys/class/gpio/gpio$red_led/direction
fi
