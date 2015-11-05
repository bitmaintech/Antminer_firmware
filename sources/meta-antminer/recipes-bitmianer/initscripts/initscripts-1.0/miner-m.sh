#!/bin/sh 

pat=/config/minermonitor.conf


while true; do

Check_RET=`pidof miner-monitor| wc -w`
echo "Check_RET ${Check_RET}" 
OFF_ON_NUM=$(grep "\"on\"" /config/minermonitor.conf | wc -l)  
echo "OFF_ON_NUM ${OFF_ON_NUM}" 
C=`pidof cgminer | wc -w`

if [ "$C" == "1" ]; then 
if [ "$Check_RET" = "0" ] && [ "$OFF_ON_NUM" = "1" ];
 then
    killall -9 miner-monitor 
    exec miner-monitor & 
    echo "restart minermonitor again!"  
fi

if  [ "$OFF_ON_NUM" = "0" ] && [ $Check_RET -gt  1 ];
 then
    killall -9 miner-monitor
    exec miner-monitor & 
    echo "kill kill client!"
fi 
fi
sleep 1m
done