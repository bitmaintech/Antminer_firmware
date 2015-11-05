#!/bin/sh

echo {

# Read network configuration
ant_minertype=AntMiner-S4
ant_nettype=
ant_netdevice=
ant_macaddr=
ant_hostname=
ant_ipaddress=
ant_netmask=
ant_gateway=
ant_dnsservers=

ant_minertype=`sed -n 2p /usr/bin/compile_time`
ant_system_filesystem_version=`sed -n 1p /usr/bin/compile_time`

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
	
	ant_host_name=`cat /config/network.conf | grep hostname`
	ant_host_name=${ant_host_name/hostname=/}
	ant_ipaddress=`cat /config/network.conf | grep ipaddress`
	ant_ipaddress=${ant_ipaddress/ipaddress=/}
	ant_netmask=`cat /config/network.conf | grep netmask`
	ant_netmask=${ant_netmask/netmask=/}
	ant_gateway=`cat /config/network.conf | grep gateway`
	ant_gateway=${ant_gateway/gateway=/}
	ant_dnsservers=`cat /config/network.conf | grep dnsservers`
	ant_dnsservers=${ant_dnsservers/dnsservers=/}
	ant_dnsservers=${ant_dnsservers//\"/}
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

echo \"minertype\":\"${ant_minertype}\",
echo \"nettype\":\"${ant_nettype}\",
echo \"netdevice\":\"${ant_netdevice}\",
echo \"macaddr\":\"${ant_macaddr}\",
echo \"hostname\":\"${ant_host_name}\",
echo \"ipaddress\":\"${ant_ipaddress}\",
echo \"netmask\":\"${ant_netmask}\",
echo \"gateway\":\"${ant_gateway}\",
echo \"dnsservers\":\"${ant_dnsservers}\",

#Read system uptime status
ant_curtime=
ant_uptime=
ant_loadaverage=

ant_tmp=`uptime`
ant_tmp=${ant_tmp/,/ }
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		0 )
		ant_curtime=${ant_var}
		;;
		2 )
		ant_uptime=${ant_var}
		;;
	esac
	i=`expr $i + 1`
done

ant_loadaverage=${ant_tmp#*average:}
ant_loadaverage=${ant_loadaverage# }
ant_loadaverage=${ant_loadaverage% }

echo \"curtime\":\"${ant_curtime}\",
echo \"uptime\":\"${ant_uptime}\",
echo \"loadaverage\":\"${ant_loadaverage}\",

#Read system memory
ant_mem_total=
ant_mem_used=
ant_mem_free=
ant_mem_buffers=
ant_mem_cached=

ant_tmp=`free | grep Mem:`
ant_tmp=${ant_tmp/Mem:/}
i=0
for ant_var in ${ant_tmp}
do
	case ${i} in
		0 )
		ant_mem_total=${ant_var}
		;;
		1 )
		ant_mem_used=${ant_var}
		;;
		2 )
		ant_mem_free=${ant_var}
		;;
		3)
		ant_mem_cached=${ant_var}
		;;
		4 )
		ant_mem_buffers=${ant_var}
		;;
	esac
	i=`expr $i + 1`
done;

echo \"mem_total\":\"${ant_mem_total}\",
echo \"mem_used\":\"${ant_mem_used}\",
echo \"mem_free\":\"${ant_mem_free}\",
echo \"mem_buffers\":\"${ant_mem_buffers}\",
echo \"mem_cached\":\"${ant_mem_cached}\",

ant_cgminer_version=`cgminer-api -o version`
if [ "${ant_cgminer_version}" == "Socket connect failed: Connection refused" ]; then
	ant_cgminer_version=
else
	ant_cgminer_version=${ant_cgminer_version#*CGMiner=}
	ant_cgminer_version=${ant_cgminer_version%%,API=*}
fi

ant_hwv1=x
ant_hwv2=x
ant_hwv3=x
ant_hwv4=x
ant_hwv=`cgminer-api -o stats`
if [ "${ant_hwv}" != "Socket connect failed: Connection refused" ]; then
	ant_hwv1=${ant_hwv#*hwv1=}
	ant_hwv1=${ant_hwv1%%,hwv2=*}
	
	ant_hwv2=${ant_hwv#*hwv2=}
	ant_hwv2=${ant_hwv2%%,hwv3=*}
	
	ant_hwv3=${ant_hwv#*hwv3=}
	ant_hwv3=${ant_hwv3%%,hwv4=*}
	
	ant_hwv4=${ant_hwv#*hwv4=}
	ant_hwv4=${ant_hwv4%%,fan_num=*}
fi

ant_system_mode=`uname -o`
ant_system_kernel_version=`uname -srv`
#ant_system_filesystem_version=`cat /usr/bin/compile_time`
ant_minertype=`sed -n 2p /usr/bin/compile_time`
ant_system_filesystem_version=`sed -n 1p /usr/bin/compile_time`

echo \"system_mode\":\"${ant_system_mode}\",
echo \"ant_hwv1\":\"${ant_hwv1}\",
echo \"ant_hwv2\":\"${ant_hwv2}\",
echo \"ant_hwv3\":\"${ant_hwv3}\",
echo \"ant_hwv4\":\"${ant_hwv4}\",
echo \"system_kernel_version\":\"${ant_system_kernel_version}\",
echo \"system_filesystem_version\":\"${ant_system_filesystem_version}\", 
echo \"cgminer_version\":\"${ant_cgminer_version}\"

echo }
