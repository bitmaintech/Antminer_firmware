#!/bin/sh

# gpio 49 = PWR_EN
#echo 49 > /sys/class/gpio/export
# P8.39 = gpio2_12 = DC/DC reset
#echo 76 > /sys/class/gpio/export
#echo high > /sys/class/gpio/gpio76/direction
# gpio2_5 = 69 = red LED
if [ ! -e /sys/class/gpio/gpio69 ]; then
	echo 69 > /sys/class/gpio/export
fi
# gpio2_2 = 66 = green LED 
if [ ! -e /sys/class/gpio/gpio66 ]; then
	echo 66 > /sys/class/gpio/export
fi

echo '#!/bin/sh' > /sbin/reboot.safe
#echo 'i2cset -y 1 0x24 0xb 0x6b' >> /sbin/reboot.safe
#echo 'i2cset -y 1 0x24 0x16 0' >> /sbin/reboot.safe
echo 'reboot -f' >> /sbin/reboot.safe
chmod a+x /sbin/reboot.safe

# Turn ON red LED, turn ON green LED
echo high > /sys/class/gpio/gpio69/direction
echo high > /sys/class/gpio/gpio66/direction

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
        if [[ $i -gt 10 ]] ; then
                break
        fi
done
if [[ $exit_code = 0 ]] ; then
        # Turn OFF red, Turn ON green
        echo high > /sys/class/gpio/gpio66/direction
        echo low > /sys/class/gpio/gpio69/direction

	#./waas -zd
else
        # Turn ON red, Turn OFF green
        echo low > /sys/class/gpio/gpio66/direction
        echo high > /sys/class/gpio/gpio69/direction
fi
