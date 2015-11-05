#!/bin/sh

echo {

# Read miner status
ant_elapsed=
ant_ghs5s=
ant_ghsav=
ant_foundblocks=
ant_getworks=
ant_accepted=
ant_rejected=
ant_hw=
ant_utility=
ant_discarded=
ant_stale=
ant_localwork=
ant_wu=
ant_diffa=
ant_diffr=
ant_bestshare=

echo \"summary\": {

ant_tmp=`cgminer-api -o`
#ant_tmp="STATUS=S,When=1393310324,Code=11,Msg=Summary,Description=cgminer 3.12.0|SUMMARY,Elapsed=567,GHS 5s=192.56,GHS av=192.38,Found Blocks=0,Getworks=43,Accepted=66,Rejected=0,Hardware Errors=72,Utility=6.99,Discarded=78,Stale=0,Get Failures=0,Local Work=29300,Remote Failures=0,Network Blocks=1,Total MH=109044924.6528,Work Utility=2688.15,Difficulty Accepted=21504.00000000,Difficulty Rejected=0.00000000,Difficulty Stale=0.00000000,Best Share=52075,Device Hardware%=0.2827,Device Rejected%=0.0000,Pool Rejected%=0.0000,Pool Stale%=0.0000,Last getwork=1393310324|"

if [ "${ant_tmp}" == "Socket connect failed: Connection refused" ]; then
	ant_elapsed=0
	ant_ghs5s=0
	ant_ghsav=0
	ant_foundblocks=0
	ant_getworks=0
	ant_accepted=0
	ant_rejected=0
	ant_hw=0
	ant_utility=0
	ant_discarded=0
	ant_stale=0
	ant_localwork=0
	ant_wu=0
	ant_diffa=0
	ant_diffr=0
	ant_bestshare=0
else
	ant_elapsed=${ant_tmp#*Elapsed=}
	ant_elapsed=${ant_elapsed%%,GHS 5s=*}
	
	ant_ghs5s=${ant_tmp#*GHS 5s=}
	ant_ghs5s=${ant_ghs5s%%,GHS av=*}
	
	ant_ghsav=${ant_tmp#*GHS av=}
	ant_ghsav=${ant_ghsav%%,Found Blocks=*}
	
	ant_foundblocks=${ant_tmp#*Found Blocks=}
	ant_foundblocks=${ant_foundblocks%%,Getworks=*}
	
	ant_getworks=${ant_tmp#*Getworks=}
	ant_getworks=${ant_getworks%%,Accepted=*}
	
	ant_accepted=${ant_tmp#*Accepted=}
	ant_accepted=${ant_accepted%%,Rejected=*}
	
	ant_rejected=${ant_tmp#*Rejected=}
	ant_rejected=${ant_rejected%%,Hardware Errors=*}
	
	ant_hw=${ant_tmp#*Hardware Errors=}
	ant_hw=${ant_hw%%,Utility=*}
	
	ant_utility=${ant_tmp#*Utility=}
	ant_utility=${ant_utility%%,Discarded=*}
	
	ant_discarded=${ant_tmp#*Discarded=}
	ant_discarded=${ant_discarded%%,Stale=*}
	
	ant_stale=${ant_tmp#*Stale=}
	ant_stale=${ant_stale%%,Get Failures=*}
	
	ant_localwork=${ant_tmp#*Local Work=}
	ant_localwork=${ant_localwork%%,Remote Failures=*}
	
	ant_wu=${ant_tmp#*Work Utility=}
	ant_wu=${ant_wu%%,Difficulty Accepted=*}
	
	ant_diffa=${ant_tmp#*Difficulty Accepted=}
	ant_diffa=${ant_diffa%%,Difficulty Rejected=*}
	
	ant_diffr=${ant_tmp#*Difficulty Rejected=}
	ant_diffr=${ant_diffr%%,Difficulty Stale=*}
	
	ant_diffs=${ant_tmp#*Difficulty Stale=}
	ant_diffs=${ant_diffs%%,Best Share=*}
	
	ant_bestshare=${ant_tmp#*Best Share=}
	ant_bestshare=${ant_bestshare%%,Device Hardware*}
fi

echo \"elapsed\":\"${ant_elapsed}\",
echo \"ghs5s\":\"${ant_ghs5s}\",
echo \"ghsav\":\"${ant_ghsav}\",
echo \"foundblocks\":\"${ant_foundblocks}\",
echo \"getworks\":\"${ant_getworks}\",
echo \"accepted\":\"${ant_accepted}\",
echo \"rejected\":\"${ant_rejected}\",
echo \"hw\":\"${ant_hw}\",
echo \"utility\":\"${ant_utility}\",
echo \"discarded\":\"${ant_discarded}\",
echo \"stale\":\"${ant_stale}\",
echo \"localwork\":\"${ant_localwork}\",
echo \"wu\":\"${ant_wu}\",
echo \"diffa\":\"${ant_diffa}\",
echo \"diffr\":\"${ant_diffr}\",
echo \"diffs\":\"${ant_diffs}\",
echo \"bestshare\":\"${ant_bestshare}\"

echo },

ant_tmp=`cgminer-api -o pools`
#ant_tmp="STATUS=S,When=1393313007,Code=7,Msg=3 Pool(s),Description=cgminer 3.12.0|POOL=0,URL=stratum+tcp://stratum.f2pool.com:25,Status=Alive,Priority=0,Quota=1,Long Poll=N,Getworks=226,Accepted=272,Rejected=0,Discarded=404,Stale=0,Get Failures=0,Remote Failures=0,User=xuelei5151.3,Last Share Time=0:00:08,Diff=768,Diff1 Shares=145542,Proxy Type=,Proxy=,Difficulty Accepted=127104.00000000,Difficulty Rejected=0.00000000,Difficulty Stale=0.00000000,Last Share Difficulty=768.00000000,Has Stratum=true,Stratum Active=true,Stratum URL=stratum.f2pool.com,Has GBT=false,Best Share=311621,Pool Rejected%=0.0000,Pool Stale%=0.0000|POOL=1,URL=stratum+tcp://stratum.f2pool.com:3333,Status=Alive,Priority=1,Quota=1,Long Poll=N,Getworks=1,Accepted=0,Rejected=0,Discarded=0,Stale=0,Get Failures=0,Remote Failures=0,User=xuelei5151.3,Last Share Time=0,Diff=256,Diff1 Shares=0,Proxy Type=,Proxy=,Difficulty Accepted=0.00000000,Difficulty Rejected=0.00000000,Difficulty Stale=0.00000000,Last Share Difficulty=0.00000000,Has Stratum=true,Stratum Active=false,Stratum URL=,Has GBT=false,Best Share=0,Pool Rejected%=0.0000,Pool Stale%=0.0000|POOL=2,URL=http://50.31.149.57:3333,Status=Alive,Priority=2,Quota=1,Long Poll=N,Getworks=2,Accepted=0,Rejected=0,Discarded=0,Stale=0,Get Failures=0,Remote Failures=0,User=xuelei5151_3,Last Share Time=0,Diff=512,Diff1 Shares=0,Proxy Type=,Proxy=,Difficulty Accepted=0.00000000,Difficulty Rejected=0.00000000,Difficulty Stale=0.00000000,Last Share Difficulty=0.00000000,Has Stratum=true,Stratum Active=false,Stratum URL=,Has GBT=false,Best Share=0,Pool Rejected%=0.0000,Pool Stale%=0.0000|"

echo \"pools\": [

if [ "${ant_tmp}" != "Socket connect failed: Connection refused" ]; then
	ant_last_len=0
	ant_len=0
	ant_first=1
	while :;
	do
		ant_tmp=${ant_tmp#*POOL=}
		ant_len=${#ant_tmp}
	
		if [ ${ant_len} -eq ${ant_last_len} ]; then
			break
		fi
		ant_last_len=${ant_len}
		
		if [ ${ant_first} -eq 1 ]; then
			ant_first=0
		else
			echo ,
		fi
		
		echo {
		ant_pool_index=
		ant_pool_url=
		ant_pool_user=
		ant_pool_status=
		ant_pool_priority=
		ant_pool_getworks=
		ant_pool_accepted=
		ant_pool_rejected=
		ant_pool_discarded=
		ant_pool_stale=
		ant_pool_diff=
		ant_pool_diff1=
		ant_pool_diffa=
		ant_pool_diffr=
		ant_pool_diffs=
		ant_pool_lsdiff=
		ant_pool_lstime=
		
		ant_pool_index=${ant_tmp%%,URL=*}
		echo \"index\":\"${ant_pool_index}\",
		
		ant_pool_url=${ant_tmp#*URL=}
		ant_pool_url=${ant_pool_url%%,Status=*}
		echo \"url\":\"${ant_pool_url}\",
		
		ant_pool_user=${ant_tmp#*User=}
		ant_pool_user=${ant_pool_user%%,Last Share Time=*}
		echo \"user\":\"${ant_pool_user}\",
		
		ant_pool_status=${ant_tmp#*Status=}
		ant_pool_status=${ant_pool_status%%,Priority=*}
		echo \"status\":\"${ant_pool_status}\",
		
		ant_pool_priority=${ant_tmp#*Priority=}
		ant_pool_priority=${ant_pool_priority%%,Quota=*}
		echo \"priority\":\"${ant_pool_priority}\",
		
		ant_pool_getworks=${ant_tmp#*Getworks=}
		ant_pool_getworks=${ant_pool_getworks%%,Accepted=*}
		echo \"getworks\":\"${ant_pool_getworks}\",
		
		ant_pool_accepted=${ant_tmp#*Accepted=}
		ant_pool_accepted=${ant_pool_accepted%%,Rejected=*}
		echo \"accepted\":\"${ant_pool_accepted}\",
		
		ant_pool_rejected=${ant_tmp#*Rejected=}
		ant_pool_rejected=${ant_pool_rejected%%,Discarded=*}
		echo \"rejected\":\"${ant_pool_rejected}\",
		
		ant_pool_discarded=${ant_tmp#*Discarded=}
		ant_pool_discarded=${ant_pool_discarded%%,Stale=*}
		echo \"discarded\":\"${ant_pool_discarded}\",
		
		ant_pool_stale=${ant_tmp#*Stale=}
		ant_pool_stale=${ant_pool_stale%%,Get Failures=*}
		echo \"stale\":\"${ant_pool_stale}\",
		
		ant_pool_diff=${ant_tmp#*Diff=}
		ant_pool_diff=${ant_pool_diff%%,Diff1 Shares=*}
		echo \"diff\":\"${ant_pool_diff}\",
		
		ant_pool_diff1=${ant_tmp#*Diff1 Shares=}
		ant_pool_diff1=${ant_pool_diff1%%,Proxy Type=*}
		echo \"diff1\":\"${ant_pool_diff1}\",
		
		ant_pool_diffa=${ant_tmp#*Difficulty Accepted=}
		ant_pool_diffa=${ant_pool_diffa%%,Difficulty Rejected=*}
		echo \"diffa\":\"${ant_pool_diffa}\",
		
		ant_pool_diffr=${ant_tmp#*Difficulty Rejected=}
		ant_pool_diffr=${ant_pool_diffr%%,Difficulty Stale=*}
		echo \"diffr\":\"${ant_pool_diffr}\",
		
		ant_pool_diffs=${ant_tmp#*Difficulty Stale=}
		ant_pool_diffs=${ant_pool_diffs%%,Last Share Difficulty=*}
		echo \"diffs\":\"${ant_pool_diffs}\",
		
		ant_pool_lsdiff=${ant_tmp#*Last Share Difficulty=}
		ant_pool_lsdiff=${ant_pool_lsdiff%%,Has Stratum=*}
		echo \"lsdiff\":\"${ant_pool_lsdiff}\",
		
		ant_pool_lstime=${ant_tmp#*Last Share Time=}
		ant_pool_lstime=${ant_pool_lstime%%,Diff=*}
		echo \"lstime\":\"${ant_pool_lstime}\"
		echo }
	done
fi

echo ],

ant_tmp=`cgminer-api -o stats`
#ant_tmp="STATUS=S,When=1393316431,Code=70,Msg=CGMiner stats,Description=cgminer 3.12.0|STATS=0,ID=BMM0,Elapsed=6676,Calls=0,Wait=0.000000,Max=0.000000,Min=99999999.000000,baud=115200,miner_count=2,asic_count=8,timeout=40,frequency=375,voltage=5,fan_num=1,fan1=2400,fan2=0,fan3=0,fan4=0,temp_num=2,temp1=50,temp2=49,temp3=0,temp4=0,temp_avg=49,temp_max=50,Device Hardware%=0.2886,no_matching_work=0,chain_acn1=32,chain_acn2=32,chain_acn3=0,chain_acn4=0,chain_acs1=oooooooo oooooooo oooooooo oooooooo,chain_acs2=oooooooo oooooooo oooooooo oooooooo,chain_acs3=,chain_acs4=,USB Pipe=0,USB Delay=r0 0.000000 w0 0.000000,USB tmo=2271686 50=36872/68/113/2301826/663696 100=6/120/166/704/108 500=0/0/0/0/0 |STATS=1,ID=POOL0,Elapsed=6676,Calls=0,Wait=0.000000,Max=0.000000,Min=99999999.000000,Pool Calls=0,Pool Attempts=0,Pool Wait=0.000000,Pool Max=0.000000,Pool Min=99999999.000000,Pool Av=0.000000,Work Had Roll Time=false,Work Can Roll=false,Work Had Expire=false,Work Roll Time=0,Work Diff=384.00000000,Min Diff=256.00000000,Max Diff=768.00000000,Min Diff Count=9528,Max Diff Count=174022,Times Sent=520,Bytes Sent=67391,Times Recv=963,Bytes Recv=428996,Net Bytes Sent=67391,Net Bytes Recv=428996|STATS=2,ID=POOL1,Elapsed=6676,Calls=0,Wait=0.000000,Max=0.000000,Min=99999999.000000,Pool Calls=0,Pool Attempts=0,Pool Wait=0.000000,Pool Max=0.000000,Pool Min=99999999.000000,Pool Av=0.000000,Work Had Roll Time=false,Work Can Roll=false,Work Had Expire=false,Work Roll Time=0,Work Diff=256.00000000,Min Diff=256.00000000,Max Diff=256.00000000,Min Diff Count=1,Max Diff Count=1,Times Sent=2,Bytes Sent=145,Times Recv=4,Bytes Recv=1209,Net Bytes Sent=145,Net Bytes Recv=1209|STATS=3,ID=POOL2,Elapsed=6676,Calls=0,Wait=0.000000,Max=0.000000,Min=99999999.000000,Pool Calls=0,Pool Attempts=0,Pool Wait=0.000000,Pool Max=0.000000,Pool Min=99999999.000000,Pool Av=0.000000,Work Had Roll Time=false,Work Can Roll=false,Work Had Expire=false,Work Roll Time=0,Work Diff=512.00000000,Min Diff=512.00000000,Max Diff=512.00000000,Min Diff Count=1,Max Diff Count=1,Times Sent=4,Bytes Sent=269,Times Recv=8,Bytes Recv=2595,Net Bytes Sent=982,Net Bytes Recv=2595|"

echo \"devs\": [

if [ "${ant_tmp}" != "Socket connect failed: Connection refused" ]; then
	i=1
	first=1
	ant_chain_acn=
	ant_freq=
	ant_fan=
	ant_temp=
	ant_chain_acs=
	
	ant_freq=${ant_tmp#*frequency=}
	ant_freq=${ant_freq%%,voltage=*}
		
	while :;
	do
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn1=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn2=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan1=}
			ant_fan=${ant_fan%%,fan2=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp1=}
			ant_temp=${ant_temp%%,temp2=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs1=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs2=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn2=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn3=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan2=}
			ant_fan=${ant_fan%%,fan3=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp2=}
			ant_temp=${ant_temp%%,temp3=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs2=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs3=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn3=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn4=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan3=}
			ant_fan=${ant_fan%%,fan4=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp3=}
			ant_temp=${ant_temp%%,temp4=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs3=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs4=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn4=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn5=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan4=}
			ant_fan=${ant_fan%%,fan5=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp4=}
			ant_temp=${ant_temp%%,temp5=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs4=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs5=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn5=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn6=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan5=}
			ant_fan=${ant_fan%%,fan6=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp5=}
			ant_temp=${ant_temp%%,temp6=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs5=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs6=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn6=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn7=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan6=}
			ant_fan=${ant_fan%%,fan7=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp6=}
			ant_temp=${ant_temp%%,temp7=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs6=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs7=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn7=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn8=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan7=}
			ant_fan=${ant_fan%%,fan8=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp7=}
			ant_temp=${ant_temp%%,temp8=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs7=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs8=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn8=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn9=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan8=}
			ant_fan=${ant_fan%%,fan9=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp8=}
			ant_temp=${ant_temp%%,temp9=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs8=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs9=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn9=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn10=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan9=}
			ant_fan=${ant_fan%%,fan10=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp9=}
			ant_temp=${ant_temp%%,temp10=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs9=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs10=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn10=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn11=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan10=}
			ant_fan=${ant_fan%%,fan11=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp10=}
			ant_temp=${ant_temp%%,temp11=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs10=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs11=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn11=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn12=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan11=}
			ant_fan=${ant_fan%%,fan12=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp11=}
			ant_temp=${ant_temp%%,temp12=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs11=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs12=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn12=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn13=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan12=}
			ant_fan=${ant_fan%%,fan13=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp12=}
			ant_temp=${ant_temp%%,temp13=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs12=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs13=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn13=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn14=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan13=}
			ant_fan=${ant_fan%%,fan14=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp13=}
			ant_temp=${ant_temp%%,temp14=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs13=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs14=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn14=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn15=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan14=}
			ant_fan=${ant_fan%%,fan15=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp14=}
			ant_temp=${ant_temp%%,temp15=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs14=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs15=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn15=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn16=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan15=}
			ant_fan=${ant_fan%%,fan16=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp15=}
			ant_temp=${ant_temp%%,temp16=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs15=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs16=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn16=}
		ant_chain_acn=${ant_chain_acn%%,chain_acs1=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			if [ "${first}" == "1" ]; then
				first=0
			else
				echo ,
			fi
			echo {
			echo \"index\":\"${i}\",
			echo \"chain_acn\":\"${ant_chain_acn}\",
			echo \"freq\":\"${ant_freq}\",
		
			ant_fan=${ant_tmp#*fan16=}
			ant_fan=${ant_fan%%,temp_num=*}
			echo \"fan\":\"${ant_fan}\",
		
			ant_temp=${ant_tmp#*temp16=}
			ant_temp=${ant_temp%%,temp_avg=*}
			echo \"temp\":\"${ant_temp}\",
		
			ant_chain_acs=${ant_tmp#*chain_acs16=}
			ant_chain_acs=${ant_chain_acs%%,USB Pipe=*}
			echo \"chain_acs\":\"${ant_chain_acs}\"
			echo }
		fi
		
		break;
	done
fi

echo ]

echo }