#!/bin/sh
. ./cgi_lib.cgi
error=false

IFS="&"
set -- $QUERY_STRING

for i in $@; do 
	IFS="="
	set -- $i
	if [ "$2" = "" ] ; then
		# error, all fields are mandatory
		error=true
		invalid_parameter=$1
		break
	else
		if [ "$1" = "current_pw" ] ; then
			curr_pw=`urldecode $2`
			hash=`echo -n "root:antMiner Configuration:$curr_pw" | md5sum | cut -b -32`
			echo "root:antMiner Configuration:$hash" > /tmp/validate_pw.tmp.$$
			diff /config/lighttpd-htdigest.user /tmp/validate_pw.tmp.$$ > /dev/null
			if [ "$?" -ne "0" ] ; then
				error=true
				invalid_parameter=$1
				break
			fi
		fi
		if [ "$1" = "new_pw" ] ; then
			new_pw=`urldecode $2`
			if [ "$new_pw" = "" ] ; then
				error=true
			fi
		fi
		if [ "$1" = "new_pw_ctrl" ] ; then
			new_pw_ctrl=`urldecode $2`
			if [ "$new_pw_ctrl" = "" ] ; then
				error=true
			fi
		fi
	fi
done

if [ "$new_pw" != "$new_pw_ctrl" ] ; then
	error=true
fi

# Need to show new page before actually apply'ing new password
if [ "$error" = "false" ] ; then
	show_msg "Updating Password"
else
	if [ "$invalid_parameter" = "current_pw" ] ; then
		show_msg "Invalid Password"
	else
		if [ "$new_pw" = "" ] || [ "$new_pw_ctrl" = "" ] ; then	    
			show_msg "New Password must be set"
		else
			show_msg "New Password does not match"
		fi
	fi
fi
sleep 1

# Apply the new password
if [ "$error" = "false" ] ; then
	# Create new lighttpd-htdigest.user file
	hash=`echo -n "root:antMiner Configuration:$new_pw" | md5sum | cut -b -32`
	echo "root:antMiner Configuration:$hash" > \
	/config/lighttpd-htdigest.user

	#printf "$new_pw\n$new_pw_ctrl" | passwd root > /dev/null
	#if [ $? -eq 0 ] ; then
		#rm -f /config/shadow
		#mv /etc/shadow /config/shadow
		#ln -s /config/shadow /etc/shadow
    #fi
fi
