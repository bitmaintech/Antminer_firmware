#!/bin/sh

if [ ! -f /config/lighttpd-htdigest.user ] ; then
    cp /etc/lighttpd-htdigest.user /config/lighttpd-htdigest.user
fi

if [ ! -f /config/shadow.factory ] ; then
    cp -p /etc/shadow.factory /config/shadow.factory
    chmod 0400 /config/shadow.factory
fi

if [ ! -f /config/shadow ] ; then
    cp -p /config/shadow.factory /config/shadow
    chmod 0400 /config/shadow
    rm -f /etc/shadow
    ln -s /config/shadow /etc/shadow
else
    rm -f /etc/shadow
    ln -s /config/shadow /etc/shadow
fi
