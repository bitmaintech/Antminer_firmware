#!/bin/sh

/etc/init.d/dropbear stop
/etc/init.d/cgminer.sh stop

#remove configuration
rm -f /config/dropbear
rm -f /config/cgminer.conf
rm -f /config/lighttpd-htdigest.user
rm -f /config/shadow
rm -f /config/shadow.factory
rm -f /config/network.conf

# restore factory settings
/etc/init.d/kncminer_setup.sh

/etc/init.d/network.sh
/etc/init.d/dropbear start
/etc/init.d/cgminer.sh start
