#!/bin/sh

echo "starting recovery factoary"
/etc/init.d/dropbear stop
/etc/init.d/cgminer.sh stop

#remove configuration
#rm -f /config/dropbear
#rm -f /config/asic-freq.config
#rm -f /config/cgminer.conf
#rm -f /comfig/cgminer.conf.factory
#rm -f /config/lighttpd-htdigest.user
#rm -f /config/shadow
#rm -f /config/shadow.factory
#rm -f /config/network.conf
rm -rf /config/*
sync
# restore factory settings
#/etc/init.d/bitmainer_setup.sh

#/etc/init.d/network.sh
#/etc/init.d/dropbear start
#/etc/init.d/cgminer.sh start

echo "recovery factory complete, rebooting"

reboot -f
