#!/bin/sh -e
#set -x

create_default_conf_file()
{
(
cat <<'EOF'  
{
"api-switch" : "off",
"api-uid" : " ",
"auth" : "fx",
"ip_server" : "www.minerlink.com:2000",
"port_server" : "2000",
"port_local" : "4028"
}
EOF
) > /config/minermonitor.conf
}

if [ ! -f /config/minermonitor.conf ] ; then
    if [ -f /config/minermonitor.conf.factory ] ; then
		cp /config/minermonitor.conf.factory /config/minermonitor.conf
    else
		create_default_conf_file
    fi
fi

ant_result=`cat /config/minermonitor.conf`

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
<script>
EOH

echo "ant_data = ${ant_result};"

cat <<EOT
function f_get_miner_conf() {
	try
	{
		document.getElementById("monitor_off_on").value=ant_data["api-switch"];
		document.getElementById("monitor_uid").value=ant_data["api-uid"];
		document.getElementById("monitor_ip").value=ant_data["ip_server"];
	}
	catch(err)
	{
	  alert('Invalid Miner configuration file. Edit manually or reset to default.');
	}
}
function f_submit_miner_conf() {
		
	_monitor_off_on=jQuery("#monitor_off_on").val();
	_monitor_uid=jQuery("#monitor_uid").val();
	_ant_auth= ant_data.auth;
	_ip_server=jQuery("#monitor_ip").val();
	_port_server=ant_data.port_server
	_port_local=ant_data.port_local

	jQuery("#cbi_apply_cgminer_fieldset").show();
	
	jQuery.ajax({
		url: '/cgi-bin/set_monitor_conf.cgi',
		type: 'POST',
		dataType: 'json',
		timeout: 30000,
		cache: false,
		data: { _monitor_off_on:_monitor_off_on, _monitor_uid:_monitor_uid,_ant_auth:_ant_auth,_ip_server:_ip_server,_port_server:_port_server,_port_local:_port_local},
		success: function(data) {
			window.location.reload();
			check_on_off();			
		},
		error: function() {
			window.location.reload(); 
			check_on_off();
		}
	});
}

function check_on_off(){
	var obj = document.getElementById("monitor_off_on");
	var index = obj.selectedIndex;
	var value = obj.options[index].value;
	if (value == "off"){
		alert("Please input a valid account !");
	}
}

jQuery(document).ready(function() {
	f_get_miner_conf();
});
</script>
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
					<li class="tabmenu-item-system active"><a href="/cgi-bin/minerConfiguration.cgi">Miner Configuration</a></li>
					<li class="tabmenu-item-network"><a href="/cgi-bin/minerStatus.cgi">Miner Status</a></li>
					<li class="tabmenu-item-system"><a href="/network.html">Network</a></li>
				</ul>
				<br style="clear: both" />
				<div class="tabmenu2">
					<ul class="tabmenu l2">
						<li class="tabmenu-item-system"><a href="/cgi-bin/minerConfiguration.cgi">General Settings</a></li>
						<li class="tabmenu-item-system"><a href="/cgi-bin/minerAdvanced.cgi">Advanced Settings</a></li>
						<li class="tabmenu-item-system active"><a href="/cgi-bin/minerMonitor.cgi">Miner Link</a></li>
					</ul>
					<br style="clear: both" />
				</div>
			</div>
		</div>
		<div id="maincontent">
			<noscript>
				<div class="errorbox">
					<strong>Java Script required!</strong><br /> You must enable Java Script in your browser or LuCI will not work properly.
				</div>
			</noscript>
			<h2 style="padding-bottom:10px;"><a id="content" name="content">Configuration Link</a></h2>
			<span style="color:black;">Please visit</span> 
			<span style="color:blue;">www.minerlink.com</span> 
			<span style="color:black;">to get more information.</span>
			<div class="cbi-map" id="cbi-cgminer">
				<fieldset class="cbi-section" id="cbi_msg_cgminer_fieldset" style="display:none">
					<span id="cbi_msg_cgminer" style="color:red;"></span>
				</fieldset>
				<fieldset class="cbi-section" id="cbi_apply_cgminer_fieldset" style="display:none">
					<img src="/resources/icons/loading.gif" alt="Loading" style="vertical-align:middle" />
					<span id="cbi-apply-cgminer-status">Waiting for changes to be applied...</span>
				</fieldset>
				<fieldset class="cbi-section" id="cbi-cgminer-cgminer">
					<div class="cbi-section-descr"></div>
					<fieldset class="cbi-section" id="cbi-cgminer-default">
						<legend>Settings</legend>
						<div class="cbi-value" id="cbi-cgminer-default-freq">
							<label class="cbi-value-title" for="cbid.cgminer.default.freq">Off/On</label>
							<div class="cbi-value-field">
								<select id="monitor_off_on" class="cbi-input-text">   									
    								<option value="off">OFF</option>  								
    								<option value="on">ON</option>
    							</select>
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-voltage">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool3url">UID</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.voltage" id="monitor_uid" value="" />
								<span style="color:black;">Please fill your email regitered in BITMAIN.</span>							
						</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-voltage">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool3url">Server Addr</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.voltage" id="monitor_ip" value="" />
															
						</div> 
						</div> 
					</fieldset>
					<br />
				</fieldset>
				<br />
			</div>
			<div class="cbi-page-actions">
				<input class="cbi-button cbi-button-save right" type="button" onclick="f_submit_miner_conf();" value="Save&Apply" />
				<input class="cbi-button cbi-button-reset right" type="button" onclick="f_get_miner_conf();" value="Reset" />
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
