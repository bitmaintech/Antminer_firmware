#!/bin/sh

# gpio 49 = PWR_EN
echo 49 > /sys/class/gpio/export
# P8.39 = gpio2_12 = DC/DC reset
echo 76 > /sys/class/gpio/export
echo high > /sys/class/gpio/gpio76/direction
# gpio2_6 = 70 = red LED
echo 70 > /sys/class/gpio/export
# gpio2_7 = 71 = green LED 
echo 71 > /sys/class/gpio/export
# P8.7 = gpio2_2 = CONF_DONE
echo 66 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio66/direction 
# P8.8 = gpio2_3 = nCONFIG
echo 67 > /sys/class/gpio/export
echo high > /sys/class/gpio/gpio67/direction
# P8.9 = gpio2_5 = nSTATUS
echo 69 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio69/direction
# gpio1_27 = clock_enable
echo 59 > /sys/class/gpio/export
echo high > /sys/class/gpio/gpio59/direction

echo '#!/bin/sh' > /sbin/reboot.safe
#echo 'i2cset -y 1 0x24 0xb 0x6b' >> /sbin/reboot.safe
#echo 'i2cset -y 1 0x24 0x16 0' >> /sbin/reboot.safe
echo 'reboot' >> /sbin/reboot.safe
chmod a+x /sbin/reboot.safe

# Turn ON red LED, turn ON green LED
echo low > /sys/class/gpio/gpio70/direction
echo low > /sys/class/gpio/gpio71/direction

echo Starting initc emmc flasher
cd /usr/bin

exit_code=252
i=0
while [ $exit_code -eq 252 ] ; do
        echo low > /sys/class/gpio/gpio49/direction # !pwr_en
        echo low > /sys/class/gpio/gpio76/direction # reset
        sleep 1
        echo high > /sys/class/gpio/gpio76/direction # !reset
	#./initc.high //FPGA download
        exit_code=$?
        i=$((i+1))
        if [[ $i -gt 10 ]] ; then
                break
        fi
done
if [[ $exit_code = 0 ]] ; then
        # Turn OFF red, Turn ON green
        echo high > /sys/class/gpio/gpio70/direction
        echo low > /sys/class/gpio/gpio71/direction

	#./waas -zd
else
        # Turn ON red, Turn OFF green
        echo low > /sys/class/gpio/gpio70/direction
        echo high > /sys/class/gpio/gpio71/direction
fi

fail() {                                                     
	# Turn ON red, Turn OFF green                        
	echo low > /sys/class/gpio/gpio70/direction          
	echo high > /sys/class/gpio/gpio71/direction         
	sleep 5                                              
	#i2cset -y 1 0x24 0xb 0x6b                            
	#i2cset -y 1 0x24 0x16 0                              
	shutdown now                                         
}

#board id
HEADER=$(hexdump -e '8/1 "%c"' /sys/bus/i2c/devices/0-0050/eeprom -s 5 -n 3)

if [ ${HEADER} -eq 335 ] ; then
	echo "Valid EEPROM header found"
else
    echo "Invalid EEPROM header detected"
	if [ -e /etc/eeprom.dump ] ; then
		echo "Adding header to EEPROM"
		dd if=/etc/eeprom.dump of=/sys/devices/ocp.2/44e0b000.i2c/i2c-0/0-0050/eeprom
	fi  
fi

echo "Paritioning eMMC"
dd if=/dev/zero of=/dev/mmcblk1 bs=16M count=16
./omap3-mkcard.sh /dev/mmcblk1

# Re-flash MMC                                               
mkdir /mnt/sd || fail                                        
mkdir /mnt/mmc || fail                                       
#mkfs.vfat /dev/mmcblk1p1 || fail                             
mount /dev/mmcblk0p1 /mnt/sd || fail                         
mount /dev/mmcblk1p1 /mnt/mmc || fail                        
cp /mnt/sd/MLO /mnt/mmc/ || fail                             
cp /mnt/sd/am335x-boneblack-bitmainer.dtb /mnt/mmc/ || fail   
cp /mnt/sd/initramfs.bin.emmc /mnt/mmc/initramfs.bin || fail
cp /mnt/sd/u-boot.img /mnt/mmc/ || fail                     
cp /mnt/sd/uEnv.txt.emmc /mnt/mmc/uEnv.txt || fail          
cp /mnt/sd/uImage.bin /mnt/mmc/ || fail                                                                             
umount /mnt/mmc
sync

#mkfs.vfat /dev/mmcblk1p2                                    
mount /dev/mmcblk1p2 /mnt/mmc                               
cp /mnt/sd/MLO /mnt/mmc/                                    
cp /mnt/sd/am335x-boneblack-bitmainer.dtb /mnt/mmc/          
cp /mnt/sd/initramfs.bin.emmc /mnt/mmc/initramfs.bin        
cp /mnt/sd/u-boot.img /mnt/mmc/                             
cp /mnt/sd/uEnv.txt.emmc /mnt/mmc/uEnv.txt          
cp /mnt/sd/uImage.bin /mnt/mmc/                                                                            
umount /mnt/mmc    
sync

mount /dev/mmcblk1p3 /mnt/mmc                               
echo "No startup test" > /mnt/mmc/asic_test                 
umount /mnt/mmc                        
sync

# Turn OFF red, Turn ON green                               
echo high > /sys/class/gpio/gpio70/direction                
echo low > /sys/class/gpio/gpio71/direction                 
                                                            
#sleep 5                                                     
#i2cset -y 1 0x24 0xb 0x6b                                   
#i2cset -y 1 0x24 0x16 0                                     

if [ -e /sys/class/leds/beaglebone\:green\:usr0/trigger ] ; then
	echo default-on > /sys/class/leds/beaglebone\:green\:usr0/trigger
	echo default-on > /sys/class/leds/beaglebone\:green\:usr1/trigger
	echo default-on > /sys/class/leds/beaglebone\:green\:usr2/trigger
    echo default-on > /sys/class/leds/beaglebone\:green\:usr3/trigger
fi
sleep 10
#shutdown now 
