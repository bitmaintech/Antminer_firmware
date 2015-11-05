#!/bin/sh -e

# CGI output must start with at least empty line (or headers)
printf "Content-type: text/html\r\n\r\n"

cat <<-EOH
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Content-Script-Type" content="text/javascript" />
<meta http-equiv="cache-control" content="no-cache" />
<link rel="stylesheet" type="text/css" media="screen" href="/css/cascade.css" />
<!--[if IE 6]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie6.css" /><![endif]-->
<!--[if IE 7]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie7.css" /><![endif]-->
<!--[if IE 8]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie8.css" /><![endif]-->
<script type="text/javascript" src="/js/xhr.js"></script>
<script type="text/javascript" src="/js/jquery-1.10.2.js"></script>
<script type="text/javascript" src="/js/json2.min.js"></script>
<title>Ant Miner</title>
</head>
<body class="lang_en">
	<p class="skiplink">
		<span id="skiplink1"><a href="#navigation">Skip to navigation</a></span>
		<span id="skiplink2"><a href="#content">Skip to content</a></span>
	</p>
	<div id="menubar">
		<h2 class="navigation"><a id="navigation" name="navigation">Navigation</a></h2>
		<div class="clear"></div>
	</div>
	<div id="menubar" style="background-color: #0a2b40;">
		<div class="hostinfo" style="float: left; with: 500px;">
			<img src="/images/antminer_logo.png" width="92" height="50" alt="" title="" border="0" />
		</div>
		<div class="clear"></div>
	</div>
	<div id="maincontainer">
		<div id="tabmenu">
			<div class="tabmenu1">
				<ul class="tabmenu l1">
					<li class="tabmenu-item-status"><a href="/index.html">System</a></li>
					<li class="tabmenu-item-system"><a href="/cgi-bin/minerConfiguration.cgi">Miner Configuration</a></li>
					<li class="tabmenu-item-network active"><a href="/cgi-bin/minerStatus.cgi">Miner Status</a></li>
					<li class="tabmenu-item-system"><a href="/network.html">Network</a></li>
				</ul>
				<br style="clear: both" />
			</div>
		</div>
		<div id="maincontent">
			<noscript>
				<div class="errorbox">
					<strong>Java Script required!</strong><br /> You must enable Java Script in your browser or LuCI will not work properly.
				</div>
			</noscript>
			<h2 style="padding-bottom:10px;"><a id="content" name="content">Miner Status</a></h2>
			<div class="cbi-map" id="cbi-cgminerstatus">
				<!-- tblsection -->
				<fieldset class="cbi-section" id="cbi-table-table">
					<legend>Summary</legend>
					<div class="cbi-section-descr"></div>
					<div class="cbi-section-node">
						<table class="cbi-section-table">
							<tr class="cbi-section-table-titles">
								<th class="cbi-section-table-cell">Elapsed</th>
								<th class="cbi-section-table-cell">GH/S(5s)</th>
								<th class="cbi-section-table-cell">GH/S(avg)</th>
								<th class="cbi-section-table-cell">FoundBlocks</th>
<th class="cbi-section-table-cell">LocalWork</th>								
<th class="cbi-section-table-cell">Utility</th>
								
								<th class="cbi-section-table-cell">WU</th>
								<th class="cbi-section-table-cell">BestShare</th>
							</tr>
							<tr class="cbi-section-table-descr">
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
							</tr>
EOH

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

ant_tmp=`cgminer-api -o`
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
	ant_diffa=${ant_diffa%%.*}
	
	ant_diffr=${ant_tmp#*Difficulty Rejected=}
	ant_diffr=${ant_diffr%%,Difficulty Stale=*}
	ant_diffr=${ant_diffr%%.*}
	
	ant_diffs=${ant_tmp#*Difficulty Stale=}
	ant_diffs=${ant_diffs%%,Best Share=*}
	ant_diffs=${ant_diffs%%.*}
	
	ant_bestshare=${ant_tmp#*Best Share=}
	ant_bestshare=${ant_bestshare%%,Device Hardware*}
fi

if [ "${ant_elapsed}" = "" ]; then 
	ant_elapsed=0
fi

ant_days=$((${ant_elapsed}/86400))
ant_hours=$((${ant_elapsed}/3600-${ant_days}*24))
ant_minutes=$((${ant_elapsed}/60-${ant_days}*1440-${ant_hours}*60))
ant_seconds=$((${ant_elapsed}-${ant_days}*86400-${ant_hours}*3600-${ant_minutes}*60))

ant_elapsed=
if [ ${ant_days} -gt 0 ]; then
ant_elapsed=${ant_elapsed}${ant_days}d
fi
if [ ${ant_hours} -gt 0 ]; then
ant_elapsed=${ant_elapsed}${ant_hours}h
fi
if [ ${ant_minutes} -gt 0 ]; then
ant_elapsed=${ant_elapsed}${ant_minutes}m
fi
if [ ${ant_seconds} -gt 0 ]; then
ant_elapsed=${ant_elapsed}${ant_seconds}s
fi



echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_elapsed\">${ant_elapsed}</div>"
echo "<div id=\"cbip-table-1-elapsed\"></div>"
echo "</td>"
#2014-10-16############################################################################# 
ant_ghs5s_left=${ant_ghs5s%%.*}
ant_ghs5s_left=$(echo $ant_ghs5s_left| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
ant_ghs5s_right=${ant_ghs5s#*.}
ant_ghs5s_split=${ant_ghs5s_left}"."${ant_ghs5s_right}
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_ghs5s\">${ant_ghs5s_split}</div>"
echo "<div id=\"cbip-table-1-ghs5s\"></div>"
echo "</td>"

#2014-10-16############################################################################# 
ant_ghsav_left=${ant_ghsav%%.*}
ant_ghsav_left=$(echo $ant_ghsav_left| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
ant_ghsav_right=${ant_ghsav#*.}
ant_ghsav_split=${ant_ghsav_left}"."${ant_ghsav_right}
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_ghsav\">${ant_ghsav_split}</div>"
echo "<div id=\"cbip-table-1-ghsav\"></div>"
echo "</td>"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_foundblocks\">${ant_foundblocks}</div>"
echo "<div id=\"cbip-table-1-foundblocks\"></div>"
echo "</td>"
#2014-10-16############################################################################# 
ant_localwork_split=$(echo $ant_localwork| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_localwork\">${ant_localwork_split}</div>"
echo "<div id=\"cbip-table-1-localwork\"></div>"
echo "</td>"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_utility\">${ant_utility}</div>"
echo "<div id=\"cbip-table-1-utility\"></div>"
echo "</td>"

#2014-10-16############################################################################# 
ant_wu_left=${ant_wu%%.*}
ant_wu_left=$(echo $ant_wu_left| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
ant_wu_right=${ant_wu#*.}
ant_wu_split=${ant_wu_left}"."${ant_wu_right}
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_wu\">${ant_wu_split}</div>"
echo "<div id=\"cbip-table-1-wu\"></div>"
echo "</td>"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"ant_bestshare\">${ant_bestshare}</div>"
echo "<div id=\"cbip-table-1-bestshare\"></div>"
echo "</td>"
echo "</tr>"

cat <<-EOP
						</table>
					</div>
				</fieldset>
				<!-- /tblsection -->
				<!-- tblsection -->
				<fieldset class="cbi-section" id="cbi-table-table">
					<legend>Pools</legend>
					<div class="cbi-section-descr"></div>
					<div class="cbi-section-node">
						<table id="ant_pools" class="cbi-section-table">
							<tr class="cbi-section-table-titles">
								<th class="cbi-section-table-cell">Pool</th>
								<th class="cbi-section-table-cell">URL</th>
								<th class="cbi-section-table-cell">User</th>
								<th class="cbi-section-table-cell">Status</th>
                                                                 <th class="cbi-section-table-cell">Diff</th>
                                                                 <th class="cbi-section-table-cell">GetWorks</th>
                                                                 <th class="cbi-section-table-cell">Priority</th>
								 <th class="cbi-section-table-cell">Accepted</th>		
                                                                 <th class="cbi-section-table-cell">Diff1#</th>
							 	 <th class="cbi-section-table-cell">DiffA#</th>
								 <th class="cbi-section-table-cell">DiffR#</th>
								 <th class="cbi-section-table-cell">DiffS#</th>						         	
								<th class="cbi-section-table-cell">Rejected</th>
								<th class="cbi-section-table-cell">Discarded</th>
								<th class="cbi-section-table-cell">Stale</th>
								<th class="cbi-section-table-cell">LSDiff</th>
								<th class="cbi-section-table-cell">LSTime</th>
							</tr>
							<tr class="cbi-section-table-descr">
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
							</tr>
EOP

ant_tmp=`cgminer-api -o pools`
if [ "${ant_tmp}" != "Socket connect failed: Connection refused" ]; then
	ant_last_len=0
	ant_len=0
	ant_pool_getworks_total=0
	ant_pool_accepted_total=0
	ant_pool_rejected_total=0
	ant_pool_discarded_total=0
	ant_pool_stale_total=0
	ant_pool_diff1_total=0
	ant_pool_diffa_total=0
	ant_pool_diffr_total=0
	ant_pool_diffs_total=0
	while :;
	do
		ant_tmp=${ant_tmp#*POOL=}
		ant_len=${#ant_tmp}
	
		if [ ${ant_len} -eq ${ant_last_len} ]; then
			break
		fi
		ant_last_len=${ant_len}
		
		echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
		
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
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-pool\">${ant_pool_index}</div>"
		echo "<div id=\"cbip-table-1-pool\"></div>"
		echo "</td>"
		
		ant_pool_url=${ant_tmp#*URL=}
		ant_pool_url=${ant_pool_url%%,Status=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-url\">${ant_pool_url}</div>"
		echo "<div id=\"cbip-table-1-url\"></div>"
		echo "</td>"
		
		ant_pool_user=${ant_tmp#*User=}
		ant_pool_user=${ant_pool_user%%,Last Share Time=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-user\">${ant_pool_user}</div>"
		echo "<div id=\"cbip-table-1-user\"></div>"
		echo "</td>"
		
		ant_pool_status=${ant_tmp#*Status=}
		ant_pool_status=${ant_pool_status%%,Priority=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-status\">${ant_pool_status}</div>"
		echo "<div id=\"cbip-table-1-status\"></div>"
		echo "</td>"

                ant_pool_diff=${ant_tmp#*Diff=}
		ant_pool_diff=${ant_pool_diff%%,Diff1 Shares=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diff\">${ant_pool_diff}</div>"
		echo "<div id=\"cbip-table-1-diff\"></div>"
		echo "</td>"

                ant_pool_getworks=${ant_tmp#*Getworks=}
		ant_pool_getworks=${ant_pool_getworks%%,Accepted=*}
#2014-10-16 ##################################################################################### 
ant_pool_getworks_split=$(echo $ant_pool_getworks| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_getworks_total+=$ant_pool_getworks
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-getworks\">${ant_pool_getworks_split}</div>"
		echo "<div id=\"cbip-table-1-getworks\"></div>"
		echo "</td>"


		
		ant_pool_priority=${ant_tmp#*Priority=}
		ant_pool_priority=${ant_pool_priority%%,Quota=*}
#2014-10-16 ##################################################################################### 
ant_pool_priority_split=$(echo $ant_pool_priority| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-priority\">${ant_pool_priority_split}</div>"
		echo "<div id=\"cbip-table-1-priority\"></div>"
		echo "</td>"

		ant_pool_accepted=${ant_tmp#*Accepted=}
		ant_pool_accepted=${ant_pool_accepted%%,Rejected=*}
#2014-10-16 ##################################################################################### 
ant_pool_accepted_split=$(echo $ant_pool_accepted| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_accepted_total+=$ant_pool_accepted
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-accepted\">${ant_pool_accepted_split}</div>"
		echo "<div id=\"cbip-table-1-accepted\"></div>"
		echo "</td>"
		

		ant_pool_diff1=${ant_tmp#*Diff1 Shares=}
		ant_pool_diff1=${ant_pool_diff1%%,Proxy Type=*}
#2014-10-16##################################################################################### 
ant_pool_diff1_split=$(echo $ant_pool_diff1| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_diff1_total+=$ant_pool_diff1
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diff1shares\">${ant_pool_diff1_split}</div>"
		echo "<div id=\"cbip-table-1-diff1shares\"></div>"
		echo "</td>"
		
		ant_pool_diffa=${ant_tmp#*Difficulty Accepted=}
		ant_pool_diffa=${ant_pool_diffa%%,Difficulty Rejected=*}
		ant_pool_diffa=${ant_pool_diffa%%.*}
#2014-10-16 ##################################################################################### 
ant_pool_diffa_split=$(echo $ant_pool_diffa| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_diffa_total+=$ant_pool_diffa
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diffaccepted\">${ant_pool_diffa_split}</div>"
		echo "<div id=\"cbip-table-1-diffaccepted\"></div>"
		echo "</td>"
		
		ant_pool_diffr=${ant_tmp#*Difficulty Rejected=}
		ant_pool_diffr=${ant_pool_diffr%%,Difficulty Stale=*}
		ant_pool_diffr=${ant_pool_diffr%%.*}
#2014-10-16 ##################################################################################### 
ant_pool_diffr_split=$(echo $ant_pool_diffr| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_diffr_total+=$ant_pool_diffr
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diffrejected\">${ant_pool_diffr_split}</div>"
		echo "<div id=\"cbip-table-1-diffrejected\"></div>"
		echo "</td>"
		
		ant_pool_diffs=${ant_tmp#*Difficulty Stale=}
		ant_pool_diffs=${ant_pool_diffs%%,Last Share Difficulty=*}
		ant_pool_diffs=${ant_pool_diffs%%.*}
#2014-10-16 ##################################################################################### 
ant_pool_diffs_split=$(echo $ant_pool_diffs| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_diffs_total+=$ant_pool_diffs
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diffstale\">${ant_pool_diffs_split}</div>"
		echo "<div id=\"cbip-table-1-diffstale\"></div>"
		echo "</td>"		
		
		
		ant_pool_rejected=${ant_tmp#*Rejected=}
		ant_pool_rejected=${ant_pool_rejected%%,Discarded=*}
#2014-10-16##################################################################################### 
ant_pool_rejected_split=$(echo $ant_pool_rejected| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_rejected_total+=$ant_pool_rejected
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-rejected\">${ant_pool_rejected_split}</div>"
		echo "<div id=\"cbip-table-1-rejected\"></div>"
		echo "</td>"
		
		ant_pool_discarded=${ant_tmp#*Discarded=}
		ant_pool_discarded=${ant_pool_discarded%%,Stale=*}
#2014-10-16##################################################################################### 
ant_pool_discarded_split=$(echo $ant_pool_discarded| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let ant_pool_discarded_total+=$ant_pool_discarded
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-discarded\">${ant_pool_discarded_split}</div>"
		echo "<div id=\"cbip-table-1-discarded\"></div>"
		echo "</td>"
		
		ant_pool_stale=${ant_tmp#*Stale=}
		ant_pool_stale=${ant_pool_stale%%,Get Failures=*}
		let ant_pool_stale_total+=$ant_pool_stale
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-stale\">${ant_pool_stale}</div>"
		echo "<div id=\"cbip-table-1-stale\"></div>"
		echo "</td>"
				
		
		ant_pool_lsdiff=${ant_tmp#*Last Share Difficulty=}
		ant_pool_lsdiff=${ant_pool_lsdiff%%,Has Stratum=*}
		ant_pool_lsdiff=${ant_pool_lsdiff%%.*}
#2014-10-16##################################################################################### 
ant_pool_lsdiff_split=$(echo $ant_pool_lsdiff| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-lastsharedifficulty\">${ant_pool_lsdiff_split}</div>"
		echo "<div id=\"cbip-table-1-lastsharedifficulty\"></div>"
		echo "</td>"
		
		ant_pool_lstime=${ant_tmp#*Last Share Time=}
		ant_pool_lstime=${ant_pool_lstime%%,Diff=*}
if [ "$ant_pool_lstime" = 0 ];
then
 ant_pool_lstime=Never
fi

		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-lastsharetime\">${ant_pool_lstime}</div>"
		echo "<div id=\"cbip-table-1-lastsharetime\"></div>"
		echo "</td>"
	done
	
	ant_pool_diff1_ratio=0
	ant_pool_diffa_ratio=0
	
	if [ "$ant_pool_diff1_total" -ne "0" ]; then
		ant_pool_diff1_ratio=`awk 'BEGIN{printf "%.4f%\n",'$ant_hw'/'$ant_pool_diff1_total'*100}'`
	fi
	
	if [ "$ant_pool_diffa_total" -ne "0" ]; then
		ant_pool_diffa_ratio=`awk 'BEGIN{printf "%.4f%\n",'$ant_hw'/'$ant_pool_diffa_total'*100}'`
	fi
	
	echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-pool\">total</div>"
	echo "<div id=\"cbip-table-1-pool\"></div>"
	echo "</td>"
		
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-url\"></div>"
	echo "<div id=\"cbip-table-1-url\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-user\"></div>"
	echo "<div id=\"cbip-table-1-user\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-status\"></div>"
	echo "<div id=\"cbip-table-1-status\"></div>"
	echo "</td>"

        echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff\"></div>"
	echo "<div id=\"cbip-table-1-diff\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
ant_pool_getworks_total_split=$(echo $ant_pool_getworks_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-getworks\">${ant_pool_getworks_total_split}</div>"
	echo "<div id=\"cbip-table-1-getworks\"></div>"
	echo "</td>"

	
        echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-priority\"></div>"
	echo "<div id=\"cbip-table-1-priority\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
ant_pool_accepted_total_split=$(echo $ant_pool_accepted_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-accepted\">${ant_pool_accepted_total_split}</div>"
	echo "<div id=\"cbip-table-1-accepted\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
ant_pool_diff1_total_split=$(echo $ant_pool_diff1_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
        echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff1shares\">${ant_pool_diff1_total_split}</div>"
	echo "<div id=\"cbip-table-1-diff1shares\"></div>"
	echo "</td>"

#2014-10-16##################################################################################### 
ant_pool_diffa_total_split=$(echo $ant_pool_diffa_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffaccepted\">${ant_pool_diffa_total_split}</div>"
	echo "<div id=\"cbip-table-1-diffaccepted\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
ant_pool_diffr_total_split=$(echo $ant_pool_diffr_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffrejected\">${ant_pool_diffr_total_split}</div>"
	echo "<div id=\"cbip-table-1-diffrejected\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
ant_pool_diffs_total_split=$(echo $ant_pool_diffs_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffstale\">${ant_pool_diffs_total_split}</div>"
	echo "<div id=\"cbip-table-1-diffstale\"></div>"
	echo "</td>"

#2014-10-16##################################################################################### 
ant_pool_rejected_total_split=$(echo $ant_pool_rejected_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-rejected\">${ant_pool_rejected_total_split}</div>"
	echo "<div id=\"cbip-table-1-rejected\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
ant_pool_discarded_total_split=$(echo $ant_pool_discarded_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
############################################################################################### 
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-discarded\">${ant_pool_discarded_total_split}</div>"
	echo "<div id=\"cbip-table-1-discarded\"></div>"
	echo "</td>"

#2014-10-16##################################################################################### 
ant_pool_stale_total_split=$(echo $ant_pool_stale_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-stale\">${ant_pool_stale_total_split}</div>"
	echo "<div id=\"cbip-table-1-stale\"></div>"
	echo "</td>"
	
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharedifficulty\"></div>"
	echo "<div id=\"cbip-table-1-lastsharedifficulty\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharetime\"></div>"
	echo "<div id=\"cbip-table-1-lastsharetime\"></div>"
	echo "</td>"
	
	echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-pool\">HW</div>"
	echo "<div id=\"cbip-table-1-pool\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-url\">${ant_hw}</div>"
	echo "<div id=\"cbip-table-1-url\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-user\"></div>"
	echo "<div id=\"cbip-table-1-user\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-status\"></div>"
	echo "<div id=\"cbip-table-1-status\"></div>"
	echo "</td>"
	
echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff\"></div>"
	echo "<div id=\"cbip-table-1-diff\"></div>"
	echo "</td>"
	
echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-getworks\"></div>"
	echo "<div id=\"cbip-table-1-getworks\"></div>"
	echo "</td>"

	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-priority\"></div>"
	echo "<div id=\"cbip-table-1-priority\"></div>"
	echo "</td>"		
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-accepted\"></div>"
	echo "<div id=\"cbip-table-1-accepted\"></div>"
	echo "</td>"

echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff1shares\">${ant_pool_diff1_ratio}</div>"
	echo "<div id=\"cbip-table-1-diff1shares\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffaccepted\">${ant_pool_diffa_ratio}</div>"
	echo "<div id=\"cbip-table-1-diffaccepted\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffrejected\"></div>"
	echo "<div id=\"cbip-table-1-diffrejected\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffstale\"></div>"
	echo "<div id=\"cbip-table-1-diffstale\"></div>"
	echo "</td>"


	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-rejected\"></div>"
	echo "<div id=\"cbip-table-1-rejected\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-discarded\"></div>"
	echo "<div id=\"cbip-table-1-discarded\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-stale\"></div>"
	echo "<div id=\"cbip-table-1-stale\"></div>"
	echo "</td>"	
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharedifficulty\"></div>"
	echo "<div id=\"cbip-table-1-lastsharedifficulty\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharetime\"></div>"
	echo "<div id=\"cbip-table-1-lastsharetime\"></div>"
	echo "</td>"
fi

cat <<-EOD
						</table>
					</div>
				</fieldset>
				<!-- /tblsection -->
				<!-- tblsection -->
				<fieldset class="cbi-section" id="cbi-table-table">
					<legend>AntMiner</legend>
					<div class="cbi-section-descr"></div>
					<div class="cbi-section-node">
						<table id="ant_devs" class="cbi-section-table">
							<tr class="cbi-section-table-titles">
								<th class="cbi-section-table-cell">Chain#</th>
								<th class="cbi-section-table-cell">ASIC#</th>
								<th class="cbi-section-table-cell">Frequency</th>
								<th class="cbi-section-table-cell">Temp</th>
								<th class="cbi-section-table-cell">ASIC status</th>
							</tr>
							<tr class="cbi-section-table-descr">
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
							</tr>
EOD

ant_fan1=0
ant_fan2=0
ant_fan3=0
ant_fan4=0

ant_tmp=`cgminer-api -o stats`
if [ "${ant_tmp}" != "Socket connect failed: Connection refused" ]; then
	i=1
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
			ant_fan=${ant_tmp#*fan1=}
			ant_fan=${ant_fan%%,fan2=*}
			ant_fan1=${ant_fan}
		
			ant_temp=${ant_tmp#*temp1=}
			ant_temp=${ant_temp%%,temp2=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs1=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs2=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn2=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn3=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then		
			ant_fan=${ant_tmp#*fan2=}
			ant_fan=${ant_fan%%,fan3=*}
			ant_fan2=${ant_fan}
		
			ant_temp=${ant_tmp#*temp2=}
			ant_temp=${ant_temp%%,temp3=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs2=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs3=*}

			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn3=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn4=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then		
			ant_fan=${ant_tmp#*fan3=}
			ant_fan=${ant_fan%%,fan4=*}
			ant_fan3=${ant_fan}
		
			ant_temp=${ant_tmp#*temp3=}
			ant_temp=${ant_temp%%,temp4=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs3=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs4=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn4=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn5=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan4=}
			ant_fan=${ant_fan%%,fan5=*}
			ant_fan4=${ant_fan}
		
			ant_temp=${ant_tmp#*temp4=}
			ant_temp=${ant_temp%%,temp5=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs4=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs5=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn5=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn6=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan5=}
			ant_fan=${ant_fan%%,fan6=*}
		
			ant_temp=${ant_tmp#*temp5=}
			ant_temp=${ant_temp%%,temp6=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs5=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs6=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"			
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn6=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn7=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan6=}
			ant_fan=${ant_fan%%,fan7=*}
		
			ant_temp=${ant_tmp#*temp6=}
			ant_temp=${ant_temp%%,temp7=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs6=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs7=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn7=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn8=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan7=}
			ant_fan=${ant_fan%%,fan8=*}
		
			ant_temp=${ant_tmp#*temp7=}
			ant_temp=${ant_temp%%,temp8=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs7=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs8=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn8=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn9=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan8=}
			ant_fan=${ant_fan%%,fan9=*}
		
			ant_temp=${ant_tmp#*temp8=}
			ant_temp=${ant_temp%%,temp9=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs8=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs9=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
			
##############8888888888888888888888888888888888888888
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
                        echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn9=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn10=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan9=}
			ant_fan=${ant_fan%%,fan10=*}
		
			ant_temp=${ant_tmp#*temp9=}
			ant_temp=${ant_temp%%,temp10=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs9=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs10=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn10=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn11=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan10=}
			ant_fan=${ant_fan%%,fan11=*}
		
			ant_temp=${ant_tmp#*temp10=}
			ant_temp=${ant_temp%%,temp11=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs10=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs11=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"
#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn11=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn12=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan11=}
			ant_fan=${ant_fan%%,fan12=*}
		
			ant_temp=${ant_tmp#*temp11=}
			ant_temp=${ant_temp%%,temp12=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs11=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs12=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"

#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn12=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn13=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan12=}
			ant_fan=${ant_fan%%,fan13=*}
		
			ant_temp=${ant_tmp#*temp12=}
			ant_temp=${ant_temp%%,temp13=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs12=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs13=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"

#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn13=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn14=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan13=}
			ant_fan=${ant_fan%%,fan14=*}
		
			ant_temp=${ant_tmp#*temp13=}
			ant_temp=${ant_temp%%,temp14=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs13=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs14=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"

#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn14=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn15=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan14=}
			ant_fan=${ant_fan%%,fan15=*}
		
			ant_temp=${ant_tmp#*temp14=}
			ant_temp=${ant_temp%%,temp15=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs14=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs15=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"

#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn15=}
		ant_chain_acn=${ant_chain_acn%%,chain_acn16=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan15=}
			ant_fan=${ant_fan%%,fan16=*}
		
			ant_temp=${ant_tmp#*temp15=}
			ant_temp=${ant_temp%%,temp16=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs15=}
			ant_chain_acs=${ant_chain_acs%%,chain_acs16=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>"

#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		i=`expr $i + 1`
		ant_chain_acn=
		ant_fan=
		ant_temp=
		ant_chain_acs=
		
		ant_chain_acn=${ant_tmp#*chain_acn16=}
		ant_chain_acn=${ant_chain_acn%%,chain_acs1=*}
		if [ -n ${ant_chain_acn} -a ${ant_chain_acn} != "0" ]; then
			ant_fan=${ant_tmp#*fan16=}
			ant_fan=${ant_fan%%,temp_num=*}
		
			ant_temp=${ant_tmp#*temp16=}
			ant_temp=${ant_temp%%,temp_avg=*}
		
			ant_chain_acs=${ant_tmp#*chain_acs16=}
			ant_chain_acs=${ant_chain_acs%%,USB Pipe=*}
			
			echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-chain\">${i}</div>"
			echo "<div id=\"cbip-table-1-chain\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-asic\">${ant_chain_acn}</div>"
			echo "<div id=\"cbip-table-1-asic\"></div>"
			echo "</td>" 

#2014-10-16##################################################################################### 
ant_freq_split=$(echo $ant_freq| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-frequency\">${ant_freq_split}</div>"
			echo "<div id=\"cbip-table-1-frequency\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-temp\">${ant_temp}</div>"
			echo "<div id=\"cbip-table-1-temp\"></div>"
			echo "</td>"
			echo "<td class=\"cbi-value-field\">"
			echo "<div id=\"cbi-table-1-status\">${ant_chain_acs}</div>"
			echo "<div id=\"cbip-table-1-status\"></div>"
			echo "</td>"
			echo "</tr>"
		fi
		
		break;
	done
fi

cat <<-EOF
						</table>
					</div>
					<div class="cbi-section-node" style="margin-top:8px;">
						<table id="ant_fans" class="cbi-section-table">
							<tr class="cbi-section-table-titles">
								<th class="cbi-section-table-cell" style="width:10%;">Fan#</th>
								<th class="cbi-section-table-cell">Fan1</th>
								<th class="cbi-section-table-cell">Fan2</th>
								<th class="cbi-section-table-cell">Fan3</th>
								<th class="cbi-section-table-cell">Fan4</th>
							</tr>
							<tr class="cbi-section-table-row">
								<th class="cbi-rowstyle-1 cbi-value-field">Speed (r/min)</th>
EOF
			echo "</td>"
#2014-10-16##################################################################################### 
ant_fan1_split=$(echo $ant_fan1| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
ant_fan2_split=$(echo $ant_fan2| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
ant_fan3_split=$(echo $ant_fan3| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
ant_fan4_split=$(echo $ant_fan4| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
echo "<td id=\"ant_fan1\" class=\"cbi-rowstyle-1 cbi-value-field\">${ant_fan1_split}</td>"
echo "<td id=\"ant_fan2\" class=\"cbi-rowstyle-1 cbi-value-field\">${ant_fan2_split}</td>"
echo "<td id=\"ant_fan3\" class=\"cbi-rowstyle-1 cbi-value-field\">${ant_fan3_split}</td>"
echo "<td id=\"ant_fan4\" class=\"cbi-rowstyle-1 cbi-value-field\">${ant_fan4_split}</td>"

cat <<-EOT
							</tr>
						</table>
					</div>
				</fieldset>
				<!-- /tblsection -->
				<br />
			</div>
			<div class="clear"></div>
		</div>
	</div>
	<div class="clear"></div>
	<div style="text-align: center; bottom: 0; left: 0; height: 1.5em; font-size: 80%; margin: 0; padding: 5px 0px 2px 8px; background-color: #918ca0; width: 100%;">
		<font style="color:#fff;">Copyright &copy; 2013-2014, Bitmain Technologies</font>
	</div>
</body>
</html>
EOT
