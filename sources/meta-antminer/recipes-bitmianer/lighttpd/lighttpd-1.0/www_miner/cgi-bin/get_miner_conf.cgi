#!/bin/sh
#set -x

create_default_conf_file()
{
(
cat <<'EOF'
{
"pools" : [
{
"url" : "192.168.110.30:3333",
"user" : "antminer_1",
"pass" : "123"
},
{
"url" : "http://stratum.antpool.com:3333",
"user" : "antminer_1",
"pass" : "123"
},
{
"url" : "50.31.149.57:3333",
"user" : "antminer_1",
"pass" : "123"
}
]
,
"api-listen" : true,
"api-network" : true,
"api-allow" : "W:0/0",
"bitmainbeeper" : true,
"bitmaintempoverctrl" : true
}

EOF
) > /config/cgminer.conf
}

if [ ! -f /config/cgminer.conf ] ; then
    if [ -f /config/cgminer.conf.factory ] ; then
		cp /config/cgminer.conf.factory /config/cgminer.conf
    else
		create_default_conf_file
    fi
fi

cat /config/cgminer.conf
