#!/bin/sh -e
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
"bitmain-freq": "7:200:0782",
"bitmain-voltage": ""
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

ant_result=`cat /config/cgminer.conf`

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
		document.getElementById("ant_freq").value=ant_data["bitmain-freq"];
		
	}
	catch(err)
	{
		alert('Invalid Miner configuration file. Edit manually or reset to default.');
	}
}
function f_submit_miner_conf() {
	_ant_pool1url = "192.168.110.30:3333";
	_ant_pool1user = "antminer_1";
	_ant_pool1pw = "123";
	_ant_pool2url = "http://stratum.antpool.com:3333";
	_ant_pool2user = "antminer_1";
	_ant_pool2pw = "123";
	_ant_pool3url = "50.31.149.57:3333";
	_ant_pool3user = "antminer_1";
	_ant_pool3pw = "123";
	_ant_nobeeper = "false";
	_ant_notempoverctrl = "false";
	try
	{
		for(var i = 0; i < ant_data.pools.length; i++) {
			switch(i) {
			case 0:
				_ant_pool1url = ant_data.pools[i].url;
				_ant_pool1user = ant_data.pools[i].user;
				_ant_pool1pw = ant_data.pools[i].pass;
				break;
			case 1:
				_ant_pool2url = ant_data.pools[i].url;
				_ant_pool2user = ant_data.pools[i].user;
				_ant_pool2pw = ant_data.pools[i].pass;
				break;
			case 2:
				_ant_pool3url = ant_data.pools[i].url;
				_ant_pool3user = ant_data.pools[i].user;
				_ant_pool3pw = ant_data.pools[i].pass;
				break;
			}
		}
		if(ant_data["bitmain_nobeeper"]) {
			_ant_nobeeper = "true";
		} else {
			_ant_nobeeper = "false";
		}
		if(ant_data["bitmain_notempoverctrl"]) {
			_ant_notempoverctrl = "true";
		} else {
			_ant_notempoverctrl = "false";
		}
	}
	catch(err)
	{
		alert('Invalid Miner configuration file. Edit manually or reset to default.'+err);
	}

	_ant_freq=jQuery("#ant_freq").val();
	

	jQuery("#cbi_apply_cgminer_fieldset").show();

	jQuery.ajax({
		url: '/cgi-bin/set_miner_conf.cgi',
		type: 'POST',
		dataType: 'json',
		timeout: 30000,
		cache: false,
		data: {_ant_pool1url:_ant_pool1url, _ant_pool1user:_ant_pool1user, _ant_pool1pw:_ant_pool1pw,_ant_pool2url:_ant_pool2url, _ant_pool2user:_ant_pool2user, _ant_pool2pw:_ant_pool2pw,_ant_pool3url:_ant_pool3url, _ant_pool3user:_ant_pool3user, _ant_pool3pw:_ant_pool3pw, _ant_nobeeper:_ant_nobeeper, _ant_notempoverctrl:_ant_notempoverctrl, _ant_freq:_ant_freq},
		success: function(data) {
			window.location.reload();
		},
		error: function() {
			window.location.reload();
		}
	});
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
						<li class="tabmenu-item-system active"><a href="/cgi-bin/minerAdvanced.cgi">Advanced Settings</a></li>
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
			<h2 style="padding-bottom:10px;"><a id="content" name="content">Miner Advanced Configuration</a></h2>
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
							<label class="cbi-value-title" for="cbid.cgminer.default.freq">Frequency</label>
							<div class="cbi-value-field">
								<select id="ant_freq" class="cbi-input-text">
									<option value="3:500:0981">500M</option>
                                    <option value="3:487.5:1305">487.5M</option>
                                    <option value="3:475:0901">475M</option>
                                    <option value="3:462.5:1205">462.5M</option>
								    <option value="3:450:0881">450M</option>
								    <option value="3:437.5:1105">437.5M</option>
								    <option value="3:425:0801">425M</option>
    								<option value="3:412.5:1005">412.5M</option>
									<option value="3:400:0f82">400M</option>
                                    <option value="3:393.75:1f06">393.75M</option>
                                    <option value="3:387.5:0f02">387.5M</option>
                                    <option value="4:381.25:1e06">381.25M</option>
                                    <option value="4:375:0e82">375M</option>
                                    <option value="4:368.75:1d06">368.75M</option>
                                    <option value="4:362.5:0e02">362.5M</option>
                                    <option value="4:356.25:1c06">356.25M</option>
								    <option value="4:350:0d82">350M</option>
								    <option value="4:343.75:1b06">343.75M</option>
								    <option value="4:337.5:0d02">337.5M</option>
								    <option value="4:331.25:1a06">331.25M</option>
								    <option value="4:325:0c82">325M</option>
								    <option value="4:318.75:1906">318.75M</option>
    								<option value="5:312.5:0c02">312.5M</option>
								    <option value="5:306.25:1806">306.25M</option>
									<option value="5:300:0b82">300M</option>
    								<option value="5:293.75:1706">293.75M</option>
								    <option value="5:287.5:0b02">287.5M</option>
    								<option value="5:281.25:1606">281.25M</option>
								    <option value="5:275:0a82">275M</option>
									<option value="5:268.75:1506">268.75M</option>
    								<option value="5:262.5:0a02">262.5M</option>
								    <option value="6:256.25:1406">256.25M</option>
    								<option value="6:250:0982">250M</option>
    								<option value="6:243.75:1306">243.75M</option>
    								<option value="6:237.5:1286">237.5M</option>
    								<option value="6:231.25:1206">231.25M</option>
    								<option value="6:225:0882">225M</option>
    								<option value="7:218.75:1106">218.75M</option>
    								<option value="7:212.5:1086">212.5M</option>
    								<option value="7:206.25:1006">206.25M</option>
    								<option value="7:200:0782">200M (default)</option>
    								<option value="8:193.75:0f03">193.75M</option>
    								<option value="8:187.5:0e83">187.5M</option>
    								<option value="8:181.25:0e03">181.25M</option>
    								<option value="8:175:0d83">175M</option>
    								<option value="9:168.75:1a87">168.75M</option>
    								<option value="9:162.5:0c83">162.5M</option>
									<option value="10:156.25:0c03">156.25M</option>
									<option value="10:150:0b83">150M</option>
									<option value="10:143.75:1687">143.75M</option>
									<option value="11:137.5:0a83">137.5M</option>
    								<option value="11:131.25:0a03">131.25M</option>
									<option value="12:125:0983">125M</option>
									<option value="13:118.75:0903">118.75M</option>
									<option value="13:112.5:0883">112.5M</option>
									<option value="14:106.25:0803">106.25M</option>
    								<option value="15:100:0783">100M</option>
    							</select>
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
