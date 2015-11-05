#!/bin/sh
#set -x

freq_config="/config/best_freq"
cgminer_config="/config/cgminer.conf"
cgminer_restart="/etc/init.d/cgminer.sh restart"
chip_core=55
startup_time=5m
stable_time_chip=30s
stable_times=120
max_freq=400
min_freq=349
default_freq=350
step=6.25
max_hw=0.5
temp_nums=2
chain_nums=2
max_temp=70
clear_best_freq=true

get_chip_num()
{
    ant_tmp=`cgminer-api -o stats`
	chain_num=1
	chip_num=0
    while [ $chain_num -le $chain_nums ];do
		ant_tmp=${ant_tmp#*chain_acs$chain_num=}
		let chain_num=$chain_num+1
		c_chip_num=${ant_tmp%%,*chain_acs$chain_num=*}
		let chip_num=`echo $c_chip_num | grep -o "o" |wc -w`+$chip_num
    done
}

get_freq()
{
    ant_tmp=`cgminer-api -o stats`   
    freq=${ant_tmp#*frequency=}                                                         
    freq=${freq%%,voltage=*}
}

get_freq_string()
{
    case $1 in
	500) freq_string="3:500:0981";;
	487.5) freq_string="3:487.5:1305";;
	475) freq_string="3:475:0901";;
	462.5) freq_string="3:462.5:1205";;
	450) freq_string="3:450:0881";;
	437.5) freq_string="3:437.5:1105";;
	425) freq_string="3:425:0801";;
	412.5) freq_string="3:412.5:1005";;
	400) freq_string="3:400:0f82";;
	393.75) freq_string="3:393.75:1f06";;
	387.5) freq_string="3:387.5:0f02";;
	381.25) freq_string="3:381.25:1e06";;
	375) freq_string="3:375:0e82";;
	368.75) freq_string="3:368.75:1d06";;
	362.5) freq_string="3:362.5:0e02";;
	356.25) freq_string="3:356.25:1c06";;
	350) freq_string="3:350:0d82";;
	343.75) freq_string="3:343.75:1b06";;
	337.5) freq_string="3:337.5:0d02";;
	331.25) freq_string="3:331.25:1a06";;
	325) freq_string="4:325:0c82";;
	318.75) freq_string="4:318.75:1906";;
	312.5) freq_string="4:312.5:0c02";;
	306.25) freq_string="4:306.25:1806";;
	300) freq_string="4:300:0b82";;
	293.75) freq_string="4:293.75:1706";;
	287.5) freq_string="4:287.5:0b02";;
	281.25) freq_string="4:281.25:1606";;
	275) freq_string="4:275:0a82";;
	268.75) freq_string="4:268.75:1506";;
	262.5) freq_string="4:262.5:0a02";;
	256.25) freq_string="5:256.25:1406";;
	250) freq_string="5:250:0982";;
	243.75) freq_string="5:243.75:1306";;
	237.5) freq_string="5:237.5:1286";;
	231.25) freq_string="5:231.25:1206";;
	225) freq_string="5:225:0882";;
	218.75) freq_string="6:218.75:1106";;
	212.5) freq_string="6:212.5:1086";;
	206.25) freq_string="6:206.25:1006";;
	200) freq_string="6:200:0782";;
	193.75) freq_string="6:193.75:0f03";;
	187.5) freq_string="7:187.5:0e83";;
	181.25) freq_string="7:181.25:0e03";;
	175) freq_string="7:175:0d83";;
	168.75) freq_string="7:168.75:1a87";;
	162.5) freq_string="8:162.5:0c83";;
	156.25) freq_string="8:156.25:0c03";;
	150) freq_string="8:150:0b83";;
	143.75) freq_string="8:143.75:1687";;
	137.5) freq_string="9:137.5:0a83";;
	131.25) freq_string="9:131.25:0a03";;
	125) freq_string="10:125:0983";;
	118.75) freq_string="10:118.75:0903";;
	112.5) freq_string="10:112.5:0883";;
	106.25) freq_string="10:106.25:0803";;
	100) freq_string="10:100:0783";;
	*) freq_string=""
    esac
}

set_freq()
{
    `sed -i -r "s/\"bitmain-freq\" : \".*?\"/\"bitmain-freq\" : \"$1\"/g" $cgminer_config`
    $cgminer_restart
}

get_5s_rate()
{
    ant_tmp=`cgminer-api -o stats`
    ant_tmp=${ant_tmp#*GHS 5s=}                                                                                                                                       
    ghs_5s=${ant_tmp%%,GHS av=*}
}

get_avg_rate()
{
    ant_tmp=`cgminer-api -o stats`
    ant_tmp=${ant_tmp#*GHS av=}                                                                                                                                       
    ghs_avg=${ant_tmp%%,baud=*}
}

get_hw()
{
    ant_tmp=`cgminer-api -o stats`
    ant_tmp=${ant_tmp#*Hardware%=}                                                                                                                                       
    hw=${ant_tmp%%,no_matching_work=*}
}

get_temp()
{
    ant_tmp=`cgminer-api -o stats`
	temp=1
    while [ $temp -le $temp_nums ];do
		ant_tmp=${ant_tmp#*temp$temp=}
		let temp=$temp+1
		c_temp=${ant_tmp%%,*temp$temp=*}
		if [ $c_temp -ge $max_temp ]; then
			return 1
		fi
    done
	return 0
}

check_rate()
{
    sleep $1
    get_5s_rate
    get_avg_rate
    get_chip_num
    get_freq
    t_ghs_5s=`echo $chip_num $chip_core $freq | awk '{print $1*$2*$3*0.9/1000}'`
    echo $t_ghs_5s
    echo $ghs_5s
    echo $ghs_avg
    if [ `echo "$ghs_5s $t_ghs_5s" | awk '{if($1<$2) {print 0} else{print 1}}'` -eq 0 -a `echo "$ghs_avg $t_ghs_5s" | awk '{if($1<$2) {print 0} else{print 1}}'` -eq 0 ];then
        return 1
    else 
        return 0 
    fi
}

check_startup()
{
    check_rate $startup_time
    if [ $? -eq 1 ];then
		return 1
    else
		return 0
    fi
}

check_stable()
{
    times=0
    sum_rate=0
    while [ $times -lt $stable_times ];do
		sleep $stable_time_chip
        get_5s_rate
        get_hw
        sum_rate=`echo $sum_rate $ghs_5s | awk '{print $1+$2}'`
        hw_cmp=`echo $hw $max_hw | awk '{if($1>$2) {print 0} else {print 1}}'`
        if [ $hw_cmp -eq 0 ];then
            return 1
        fi
		let times=$times+1
    done
	
	get_temp
	if [ $? -eq 1 ];then
	    return 1
	fi
	
    get_chip_num
    get_freq
    t_ghs_5s=`echo $chip_num $chip_core $freq | awk '{print $1*$2*$3*0.95/1000}'`   
    if [ `echo $sum_rate $times $t_ghs_5s | awk '{if(($1/$2)>$3) {print 1} else {print 0}}'` -eq 1 ];then
		return 0
    else
		echo " " > $freq_config
		return 1
    fi
}
    
get_next_freq()
{   
    if [ $1 == "ADD" ];then
		while [ `echo "$freq $max_freq $step" | awk '{if(($1+$3)<$2) {print 0} else{print 1}}'` -eq 0 ];do
			freq=`echo "$freq $step" |awk '{ print $1+$2 }'`
			get_freq_string $freq
			if [ "$freq_string" != "" ];then
				return 0
			fi
		done
    elif [ $1 == "SUB" ];then
		while [ `echo "$freq $min_freq $step" | awk '{if(($1-$3)>$2) {print 0} else{print 1}}'` -eq 0 ];do                                                                             
			freq=`echo "$freq $step" |awk '{ print $1-$2 }'`                                  
			get_freq_string $freq                                                             
			if [ "$freq_string" !=  "" ];then                                                        
				return 0                                                                         
			fi                                                                                
		done   
    fi
    return 1
}

set_defautl_freq()
{
    freq=$default_freq
    get_freq_string $freq
    set_freq $freq_string
}

create_default_conf_file()
{
(cat <<EOF 
{
"ant_frequency_auto" : "on"
}
EOF
)> /config/auto_freq.conf
}

main()
{
    if [ `ps | grep auto_freq.sh | wc -l` -ge 4 ];then        
        return 1                                        
    fi 
    
    if [ ! -f /config/auto_freq.conf ] ; then
        if [ -f /etc/auto_freq.conf.factory ] ; then
                cp /etc/auto_freq.conf.factory /config/auto_freq.conf
        else
                create_default_conf_file
        fi
    fi   
    
    OFF_ON_NUM=$(grep "\"on\"" /config/auto_freq.conf | wc -l)
    if  [ "$OFF_ON_NUM" == "0" ];then
        return 1
    fi
    
    sleep 5m
    
    while true; do
        C=`pidof cgminer | wc -w`
        if [ "$C" == "1" ]; then
            break
        fi
        sleep 1m
    done
	
	if [ $clear_best_freq == "true" ];then
		echo "" > /config/best_freq
		sed -r -i "s/^clear_best_freq=true$/clear_best_freq=false/g" $0
	fi
	
    if [ `grep -c "^[0-9]\{1,3\}.\{0,1\}[0-9]\{0,2\}" /config/best_freq` == "1" ];then
		freq=`cat /config/best_freq`
		get_freq_string $freq
        set_freq $freq_string
        check_startup
		if [ $? -ne 1 ];then
			check_stable
			if [ $? -ne 1 ];then
                return 0
			fi
		fi
	fi
	
	get_temp
	if [ $? -eq 1 ];then
	    return 1
	fi
	
    ok="no"
    get_freq
	
	if [ $freq -lt 350 ];then
		freq=350
	fi
	
    get_next_freq "ADD"
	if [ $? -ne 0 ];then
		freq=350
	fi
	get_freq_string $freq
	set_freq $freq_string
    while [ true ] ;do
		check_startup
		if [ $? -ne 0 ];then
			get_next_freq "SUB"
			if [ $? -ne 0 ];then
			    echo "" > $freq_config
                set_defautl_freq
                return 1
			fi
			if [ $ok == "ok" ];then
				get_freq_string $freq
				set_freq $freq_string
				echo $freq > $freq_config
				return 0
			fi
		else
			check_stable
			if [ $? -ne 0 ];then
				get_next_freq "SUB"
				if [ $? -eq 1 ];then                                                                          
					echo "" > $freq_config
					set_defautl_freq  
					return 1                                                                
				fi
				if [ $ok == "ok" ];then
					get_freq_string $freq
					set_freq $freq_string
					echo $freq > $freq_config
					return 0
				fi
			else
				ok="ok"
				now_freq=$freq
				get_next_freq "ADD"
				if [ $? -ne 0 ];then
					echo $now_freq > $freq_config
					return 0
				fi
			fi
		fi
		get_freq_string $freq
		set_freq $freq_string
    done
}

main
