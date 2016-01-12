#!/bin/sh
#set -x

create_default_auto_fre_file()
{
(
cat <<'EOF'
{
"ant_frequency_auto" : "on"
}
EOF
) > /config/auto_freq.conf
}

if [ ! -f /config/auto_freq.conf ] ; then
	create_default_auto_fre_file
fi
cat /config/auto_freq.conf
