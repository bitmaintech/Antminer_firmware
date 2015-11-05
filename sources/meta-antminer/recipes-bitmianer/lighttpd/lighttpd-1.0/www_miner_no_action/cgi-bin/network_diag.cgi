#!/bin/sh
#set -x

ant_type=
ant_ipaddress=

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
		ant_type=${ant_var/_ant_type=/}
		;;
		1 )
		ant_ipaddress=${ant_var/_ant_ipaddress=/}
		;;
	esac
	i=`expr $i + 1`
done

echo "${ant_type} ${ant_ipaddress}"

if [ "${ant_type}" == "ping" ]; then
	ping ${ant_ipaddress} -c 6
elif [ "${ant_type}" == "traceroute" ]; then
	traceroute ${ant_ipaddress}
elif [ "${ant_type}" == "nslookup" ]; then
	nslookup ${ant_ipaddress}
else
	echo "${ant_type} ${ant_ipaddress}"
fi

