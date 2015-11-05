#!/bin/sh
#set -x


monitor_off_on=
monitor_uid=
ant_auth=
ant_ip_server=
ant_port_server=
ant_port_local=


ant_input=`cat /dev/stdin`
ant_tmp=${ant_input//&/ }
i=0
for ant_var in ${ant_tmp}
do
	ant_var=${ant_var//+/ }
	ant_var=${ant_var//%23/#}
	ant_var=${ant_var//%24/$}
	ant_var=${ant_var//%25/%}
	ant_var=${ant_var//%26/&}
	ant_var=${ant_var//%2C/,}
	ant_var=${ant_var//%2B/+}
	ant_var=${ant_var//%3A/:}
	ant_var=${ant_var//%3B/;}
	ant_var=${ant_var//%3C/<}
	ant_var=${ant_var//%3D/=}
	ant_var=${ant_var//%3E/>}
	ant_var=${ant_var//%3F/?}
	ant_var=${ant_var//%40/@}
	ant_var=${ant_var//%5B/[}
	ant_var=${ant_var//%5D/]}
	ant_var=${ant_var//%5E/^}
	ant_var=${ant_var//%7B/\{}
	ant_var=${ant_var//%7C/|}
	ant_var=${ant_var//%7D/\}}
	ant_var=${ant_var//%2F/\/}
	#ant_var=${ant_var//%22/\"}
	#ant_var=${ant_var//%5C/\\}
	case ${i} in
		0 )
		monitor_off_on=${ant_var/_monitor_off_on=/}
		;;
		1 )
		monitor_uid=${ant_var/_monitor_uid=/}
		;;
		2 )
		ant_auth=${ant_var/_ant_auth=/}
		;;
		3 )
		ant_ip_server=${ant_var/_ip_server=/}
		;;
		4 )
		ant_port_server=${ant_var/_port_server=/}
		;;
		5 )
		ant_port_local=${ant_var/_port_local=/}
		;;
	esac
	i=`expr $i + 1`
done

echo "{"					                	>  /config/minermonitor.conf
echo "\"api-switch\" : \"${monitor_off_on}\","		>> /config/minermonitor.conf
echo "\"api-uid\" : \"${monitor_uid}\","		>> /config/minermonitor.conf
echo "\"auth\" : \"${ant_auth}\","			>> /config/minermonitor.conf
echo "\"ip_server\" : \"${ant_ip_server}\","		>> /config/minermonitor.conf
echo "\"port_server\" : \"${ant_port_server}\","		>> /config/minermonitor.conf
echo "\"port_local\" : \"${ant_port_local}\""		>> /config/minermonitor.conf
echo "}"                                              >> /config/minermonitor.conf

sleep 1s 


  killall -9 miner-monitor 
  sleep 1s
  exec miner-monitor & 
  echo "restart minermonitor again!"  

 

sleep 1s 
echo "ok"