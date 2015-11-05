#!/bin/sh -e

file=antminer_backup_`date +%Y-%m-%d_%H%M%S`.tar
dir=/tmp/backup$$
bkup_files="advanced.conf \
    dropbear \
    dropbear_rsa_host_key \
    led-blink.conf \
    lighttpd-htdigest.user \
    network.conf \
    shadow \
    shadow.factory \
    cgminer.conf \
    cgminer.conf.factory"


trap atexit 0

atexit() {
	rm -rf $dir

	sync
	if [ ! $ok ]; then
	    print "<h1>Create backup failed</h1>"
	fi
}

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
<div class="hostinfo" style="float:left;with:500px;">
	<img src="/images/antminer_logo.png" width="92" height="50" alt="" title="" border="0" />
</div>
<div class="clear"></div>
</div>
<div id="maincontainer">
	<div id="tabmenu">
	<div class="tabmenu1">
	<ul class="tabmenu l1">
		<li class="tabmenu-item-status active"><a href="/index.html">System</a></li>
		<li class="tabmenu-item-system"><a href="/cgi-bin/minerConfiguration.cgi">Miner Configuration</a></li>
		<li class="tabmenu-item-network"><a href="/cgi-bin/minerStatus.cgi">Miner Status</a></li>
		<li class="tabmenu-item-system"><a href="/network.html">Network</a></li>
	</ul>
	<br style="clear:both" />
	<div class="tabmenu2">
	<ul class="tabmenu l2">
		<li class="tabmenu-item-system"><a href="/index.html">Overview</a></li>
		<li class="tabmenu-item-system"><a href="/administration.html">Administration</a></li>
		<li class="tabmenu-item-admin"><a href="/monitor.html">Monitor</a></li>
		<li class="tabmenu-item-packages"><a href="/kernelLog.html">Kernel Log</a></li>
		<li class="tabmenu-item-startup active"><a href="/upgrade.html">Upgrade</a></li>
		<li class="tabmenu-item-crontab"><a href="/reboot.html">Reboot</a></li>
	</ul>
	<br style="clear:both" />
	</div>
	</div>
	</div>
		<div id="maincontent">
			<noscript>
				<div class="errorbox">
					<strong>Java Script required!</strong><br /> You must enable Java Script in your browser or LuCI will not work properly.
				</div>
			</noscript>
			<h2><a id="content" name="content">Backup</a></h2>
			<fieldset class="cbi-section">
EOH

exec 2>&1

mkdir $dir
cd $dir

for f in $bkup_files ; do
    if [ -f /config/$f ] ; then  
	cp /config/$f .
    fi
done

> ./restoreConfig.sh
echo 'mkdir -p /config/.old_config'                      >> ./restoreConfig.sh
echo 'rm -rf /config/.old_config/*'                      >> ./restoreConfig.sh
echo 'cd /config/'                                       >> ./restoreConfig.sh
echo "for f in $bkup_files ; do"                         >> ./restoreConfig.sh
echo '    if [ -f $f ] ; then'                           >> ./restoreConfig.sh
echo '	    cp $f /config/.old_config/'                  >> ./restoreConfig.sh
echo '    fi'                                            >> ./restoreConfig.sh
echo 'done'                                              >> ./restoreConfig.sh
echo 'cd - >> /dev/null'                                 >> ./restoreConfig.sh
echo 'cp * /config/'                                     >> ./restoreConfig.sh
echo 'sync'                                              >> ./restoreConfig.sh

tar cf /www/pages/$file *
if [ $? -ne 0 ] ; then
    exit
fi

cat <<EOT
<p>Save backup to PC.</p>
<table>
	<tr>
		<td>
			<form action="/$file">
				<input class="cbi-button cbi-button-down" type="submit" name="save" value="Save" />
			</form>
		</td>
		<td style="padding-left:10px;">
			<form action="/upgrade.html">
				<input class="cbi-button cbi-button-link" type="submit" name="goback" value="Go Back" />
			</form>
		</td>
	</tr>
</table>
</fieldset>
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

ok=1
