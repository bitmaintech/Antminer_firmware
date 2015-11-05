#!/bin/sh

echo {

# Read network configuration
ant_nettype=Static
ant_netdevice=
ant_macaddr=
ant_ipaddress=
ant_netmask=

if [ -s /config/network.conf ] ; then
	ant_nettype=`cat /config/network.conf | grep dhcp`
	if [ -z ant_nettype ]; then
		ant_nettype="dhcp=false"
	fi
	ant_nettype=${ant_nettype/dhcp=/}
	if [ "${ant_nettype}" == "true" ]; then
		ant_nettype=DHCP
	else
		ant_nettype=Static
	fi
fi

ant_tmp=`ifconfig | grep eth`
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		0 )
		ant_netdevice=${ant_var}
		;;
		4 )
		ant_macaddr=${ant_var}
		;;
	esac
	i=`expr $i + 1`
done

ant_tmp=`ifconfig | grep "inet addr"`
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		1 )
		ant_ipaddress=${ant_var}
		ant_ipaddress=${ant_ipaddress/addr:/}
		;;
		3 )
		ant_netmask=${ant_var}
		ant_netmask=${ant_netmask/Mask:/}
		;;
	esac
	i=`expr $i + 1`
done

echo \"nettype\":\"${ant_nettype}\",
echo \"netdevice\":\"${ant_netdevice}\",
echo \"macaddr\":\"${ant_macaddr}\",
echo \"ipaddress\":\"${ant_ipaddress}\",
echo \"netmask\":\"${ant_netmask}\",

ant_conf_nettype=${ant_nettype}
ant_conf_hostname=
ant_conf_ipaddress=
ant_conf_netmask=
ant_conf_gateway=
ant_conf_dnsservers=

if [ -s /config/network.conf ] ; then	
	ant_conf_hostname=`cat /config/network.conf | grep hostname`
	ant_conf_hostname=${ant_conf_hostname/hostname=/}
	ant_conf_ipaddress=`cat /config/network.conf | grep ipaddress`
	ant_conf_ipaddress=${ant_conf_ipaddress/ipaddress=/}
	ant_conf_netmask=`cat /config/network.conf | grep netmask`
	ant_conf_netmask=${ant_conf_netmask/netmask=/}
	ant_conf_gateway=`cat /config/network.conf | grep gateway`
	ant_conf_gateway=${ant_conf_gateway/gateway=/}
	ant_conf_dnsservers=`cat /config/network.conf | grep dnsservers`
	ant_conf_dnsservers=${ant_conf_dnsservers/dnsservers=/}
	ant_conf_dnsservers=${ant_conf_dnsservers//\"/}
fi

echo \"conf_nettype\":\"${ant_conf_nettype}\",
echo \"conf_hostname\":\"${ant_conf_hostname}\",
echo \"conf_ipaddress\":\"${ant_conf_ipaddress}\",
echo \"conf_netmask\":\"${ant_conf_netmask}\",
echo \"conf_gateway\":\"${ant_conf_gateway}\",
echo \"conf_dnsservers\":\"${ant_conf_dnsservers}\"

echo }