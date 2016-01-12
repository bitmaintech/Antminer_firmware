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
	_ant_fan_customize_switch = "false";
	_ant_fan_customize_value = "100";

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
		if(ant_data["bitmain-fan-ctrl"]) {                                                           
                        _ant_fan_customize_switch = "true";                                                  
                        _ant_fan_customize_value = ant_data["bitmain-fan-pwm"];                              
                } else {                                                                                     
                        _ant_fan_customize_switch = "false";                                                 
                        _ant_fan_customize_value = "";                                                       
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
		data: {_ant_pool1url:_ant_pool1url, _ant_pool1user:_ant_pool1user, _ant_pool1pw:_ant_pool1pw,_ant_pool2url:_ant_pool2url, _ant_pool2user:_ant_pool2user, _ant_pool2pw:_ant_pool2pw,_ant_pool3url:_ant_pool3url, _ant_pool3user:_ant_pool3user, _ant_pool3pw:_ant_pool3pw, _ant_nobeeper:_ant_nobeeper, _ant_notempoverctrl:_ant_notempoverctrl,_ant_fan_customize_switch:_ant_fan_customize_switch,_ant_fan_customize_value:_ant_fan_customize_value, _ant_freq:_ant_freq},
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
						<li class="tabmenu-item-system "><a href="/cgi-bin/minerMonitor.cgi">Miner Link</a></li>
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
<option value="100"> 100.00M </option>
<option value="125"> 125.00M </option>
<option value="150"> 150.00M </option>
<option value="175"> 175.00M </option>
<option value="200"> 200.00M </option>
<option value="225"> 225.00M </option>
<option value="250"> 250.00M </option>
<option value="275"> 275.00M </option>
<option value="300"> 300.00M </option>
<option value="325"> 325.00M </option>
<option value="350"> 350.00M </option>
<option value="375"> 375.00M </option>
<option value="400"> 400.00M </option>
<option value="404"> 404.17M </option>
<option value="406"> 406.25M </option>
<option value="408"> 408.33M </option>
<option value="412"> 412.50M </option>
<option value="416"> 416.67M </option>
<option value="418"> 418.75M </option>
<option value="420"> 420.83M </option>
<option value="425"> 425.00M </option>
<option value="429"> 429.17M </option>
<option value="431"> 431.25M </option>
<option value="433"> 433.33M </option>
<option value="437"> 437.50M </option>
<option value="437"> 437.50M </option>
<option value="441"> 441.67M </option>
<option value="443"> 443.75M </option>
<option value="445"> 445.83M </option>
<option value="450"> 450.00M </option>
<option value="454"> 454.17M </option>
<option value="456"> 456.25M </option>
<option value="458"> 458.33M </option>
<option value="462"> 462.50M </option>
<option value="466"> 466.67M </option>
<option value="468"> 468.75M </option>
<option value="470"> 470.83M </option>
<option value="475"> 475.00M </option>
<option value="479"> 479.17M </option>
<option value="481"> 481.25M </option>
<option value="483"> 483.33M </option>
<option value="487"> 487.50M </option>
<option value="491"> 491.67M </option>
<option value="493"> 493.75M </option>
<option value="495"> 495.83M </option>
<option value="500"> 500.00M </option>
<option value="504"> 504.17M </option>
<option value="506"> 506.25M </option>
<option value="508"> 508.33M </option>
<option value="512"> 512.50M </option>
<option value="516"> 516.67M </option>
<option value="518"> 518.75M </option>
<option value="520"> 520.83M </option>
<option value="525"> 525.00M </option>
<option value="529"> 529.17M </option>
<option value="531"> 531.25M </option>
<option value="533"> 533.33M </option>
<option value="537"> 537.50M </option>
<option value="543"> 543.75M </option>
<option value="550"> 550.00M </option>
<option value="556"> 556.25M </option>
<option value="562"> 562.50M </option>
<option value="568"> 568.75M </option>
<option value="575"> 575.00M </option>
<option value="581"> 581.25M </option>
<option value="587"> 587.50M </option>
<option value="593"> 593.75M </option>
<option value="600"> 600.00M </option>
<option value="606"> 606.25M </option>
<option value="612"> 612.50M </option>
<option value="618"> 618.75M </option>
<option value="625"> 625.00M </option>
<option value="631"> 631.25M </option>
<option value="637"> 637.50M </option>
<option value="643"> 643.75M </option>
<option value="650"> 650.00M </option>
<option value="656"> 656.25M </option>
<option value="662"> 662.50M </option>
<option value="668"> 668.75M </option>
<option value="675"> 675.00M </option>
<option value="681"> 681.25M </option>
<option value="687"> 687.50M </option>
<option value="693"> 693.75M </option>
<option value="700"> 700.00M </option>
<option value="706"> 706.25M </option>
<option value="712"> 712.50M </option>
<option value="718"> 718.75M </option>
<option value="725"> 725.00M </option>
<option value="731"> 731.25M </option>
<option value="737"> 737.50M </option>
<option value="743"> 743.75M </option>
<option value="750"> 750.00M </option>
<option value="756"> 756.25M </option>
<option value="762"> 762.50M </option>
<option value="768"> 768.75M </option>
<option value="775"> 775.00M </option>
<option value="781"> 781.25M </option>
<option value="787"> 787.50M </option>
<option value="793"> 793.75M </option>
<option value="800"> 800.00M </option>
<option value="812"> 812.50M </option>
<option value="825"> 825.00M </option>
<option value="837"> 837.50M </option>
<option value="850"> 850.00M </option>
<option value="862"> 862.50M </option>
<option value="875"> 875.00M </option>
<option value="887"> 887.50M </option>
<option value="900"> 900.00M </option>
<option value="912"> 912.50M </option>
<option value="925"> 925.00M </option>
<option value="937"> 937.50M </option>
<option value="950"> 950.00M </option>
<option value="962"> 962.50M </option>
<option value="975"> 975.00M </option>
<option value="987"> 987.50M </option>
<option value="1000"> 1000.00M </option>
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
