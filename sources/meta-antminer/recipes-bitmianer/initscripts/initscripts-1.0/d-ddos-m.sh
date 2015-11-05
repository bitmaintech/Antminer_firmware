#!/bin/sh 
while true; do
    Check_RET=`pidof d-ddos| wc -w`

    C=`pidof cgminer | wc -w`
    if [ "$C" == "1" ]; then 
        if [ $Check_RET -eq 0 ];then
            killall -9 d-ddos 
            exec d-ddos & 
        fi

        if [ $Check_RET -gt  1 ];then
            killall -9 d-ddos
            exec d-ddos & 
        fi 
    fi
    sleep 1m
done
