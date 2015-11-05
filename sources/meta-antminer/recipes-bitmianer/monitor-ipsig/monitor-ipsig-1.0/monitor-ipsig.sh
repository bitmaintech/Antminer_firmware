#!/bin/sh

DESC="Monitor-ipsig daemon"
DAEMON=/usr/bin/monitor-ipsig
NAME=monitor-ipsig


do_start() {
        start-stop-daemon -b -S -x  $DAEMON 
}

do_stop() {
        killall -9 monitor-ipsig
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