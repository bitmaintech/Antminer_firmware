#!/bin/sh
#set -x

ant_conf_nettype=
ant_conf_hostname=
ant_conf_ipaddress=
ant_conf_netmask=
ant_conf_gateway=
ant_conf_dnsservers=

ant_input=`cat /dev/stdin`
#ant_input=_ant_conf_nettype=Static\&_ant_conf_hostname=antMiner-A8\&_ant_conf_ipaddress=192.168.1.3\&_ant_conf_netmask=255.255.255.0\&_ant_conf_gateway=192.168.1.1\&_ant_conf_dnsservers=192.168.1.1
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
		ant_conf_nettype=${ant_var/_ant_conf_nettype=/}
		;;
		1 )
		ant_conf_hostname=${ant_var/_ant_conf_hostname=/}
		;;
		2 )
		ant_conf_ipaddress=${ant_var/_ant_conf_ipaddress=/}
		;;
		3 )
		ant_conf_netmask=${ant_var/_ant_conf_netmask=/}
		;;
		4 )
		ant_conf_gateway=${ant_var/_ant_conf_gateway=/}
		;;
		5 )
		ant_conf_dnsservers=${ant_var/_ant_conf_dnsservers=/}
		;;
	esac
	i=`expr $i + 1`
done

if [ "${ant_conf_nettype}" == "DHCP" ]; then
	echo "dhcp=true"								>  /config/network.conf
	echo "hostname=${ant_conf_hostname}"			>> /config/network.conf
else
	echo "hostname=${ant_conf_hostname}"			>  /config/network.conf
	echo "ipaddress=${ant_conf_ipaddress}"			>> /config/network.conf
	echo "netmask=${ant_conf_netmask}"				>> /config/network.conf
	echo "gateway=${ant_conf_gateway}"				>> /config/network.conf
	echo "dnsservers=\"${ant_conf_dnsservers}"\"	>> /config/network.conf
fi
sync &
/etc/init.d/network.sh
/etc/init.d/avahi restart > /dev/null

sleep 10s

echo "ok"