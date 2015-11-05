#!/bin/sh
NO_START=0

if [ ! -f /config/dropbear ] ; then
    echo NO_START=0 > /config/dropbear
fi
cp /config/dropbear /etc/default/dropbear

