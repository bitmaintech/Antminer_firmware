# $1 == msg to show
# $2 == place to return to (optional)
# $3 == time to delay (optional)

show_msg ()
{
    msg=$1
    if [ "$2" = "" ] ; then
	return_page="$HTTP_REFERER"
    else
	return_page="http://$SERVER_NAME$2"
    fi
    if [ "$3" = "" ] ; then
	delay_time=1000
    else
	delay_time=$3
    fi
    
    echo '<?xml version="1.0" encoding="utf-8"?>'
    echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'
    echo '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">'
    echo '<head>'
    echo '<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />'
    echo '<meta http-equiv="Content-Script-Type" content="text/javascript" />'
    echo '<meta http-equiv="cache-control" content="no-cache" />'
    echo '<link rel="stylesheet" type="text/css" media="screen" href="/css/cascade.css" />'
    echo '<!--[if IE 6]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie6.css" /><![endif]-->'
    echo '<!--[if IE 7]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie7.css" /><![endif]-->'
    echo '<!--[if IE 8]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie8.css" /><![endif]-->'
    echo '<script type="text/javascript" src="/js/xhr.js"></script>'
    echo '<script>'
    echo '    function delayer(){'
    echo '        window.location = "'$return_page'";'
    echo '    }'
    echo '</script>'
    echo '<title>Ant Miner</title>'
    echo '</head>'
    echo -n '<body class="lang_en" onload="setTimeout('
    echo -n "'delayer()'"
    echo -n ', '
    echo -n $delay_time
    echo ')">'
    echo '<p class="skiplink">'
    echo '<span id="skiplink1"><a href="#navigation">Skip to navigation</a></span>'
    echo '<span id="skiplink2"><a href="#content">Skip to content</a></span>'
    echo '</p>'
    echo '<div id="menubar">'
    echo '<h2 class="navigation"><a id="navigation" name="navigation">Navigation</a></h2>'
    echo '<div class="clear"></div>'
    echo '</div>'
    echo '<div id="menubar" style="background-color: #0a2b40;">'
    echo '<div class="hostinfo" style="float:left;with:500px;">'
    echo '<img src="/images/antminer_logo.png" width="92" height="50" alt="" title="" border="0" />'
    echo '</div>'
    echo '<div class="clear"></div>'
    echo '</div>'
    echo '<div>&nbsp;</div>'
    echo '<div id="maincontainer">'
    echo '<div id="maincontent">'
    echo '<div class="cbi-map" id="cbi-system">'
    echo '<fieldset class="cbi-section" id="cbi-system-_dummy" style="font-size:15px;color:red;padding-top:10px;padding-left:10px;padding-bottom:10px;">'
    echo $msg
    echo '</fieldset>'
    echo '</div>'
    echo '</div>'
    echo '</div>'
    echo '<div class="clear"></div>'
	echo '<div style="text-align: center; bottom: 0; left: 0; height: 1.5em; font-size: 80%; margin: 0; padding: 5px 0px 2px 8px; background-color: #918ca0; width: 100%;">'
	echo '<font style="color:#fff;">Copyright &copy; 2013-2014, Bitmain Technologies</font>'
	echo '</div>'
    echo '</body>'
    echo '</html>'
}

urldecode () {
    printf  '%b' "${1//%/\\x}"
}
