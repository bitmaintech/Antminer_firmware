#!/bin/bash

# Original script done by Don Darling
# Later changes by Koen Kooi and Brijesh Singh

# Revision history:
# 20090902: download from twiki
# 20090903: Weakly assign MACHINE and DISTRO
# 20090904:  * Don't recreate local.conf is it already exists
#            * Pass 'unknown' machines to OE directly
# 20090918: Fix /bin/env location
#           Don't pass MACHINE via env if it's not set
#           Changed 'build' to 'bitbake' to prepare people for non-scripted usage
#           Print bitbake command it executes
# 20091012: Add argument to accept commit id.
# 20091202: Fix proxy setup
#
# For further changes consult 'git log' or browse to:
#   http://git.angstrom-distribution.org/cgi-bin/cgit.cgi/setup-scripts/
# to see the latest revision history

# Use this till we get a maintenance branch based of the release tag

###############################################################################
# User specific vars like proxy servers
###############################################################################

#PROXYHOST=wwwgate.ti.com
#PROXYPORT=80
PROXYHOST=""

###############################################################################
# OE_BASE    - The root directory for all OE sources and development.
###############################################################################
OE_BASE=${PWD}
# incremement this to force recreation of config files
BASE_VERSION=9
OE_ENV_FILE=environment-angstrom-v2013.06

if ! git help log | grep -q no-abbrev ; then 
	echo "Your installed version of git is too old, it lacks --no-abbrev. Please install 1.7.6 or newer"
	exit 1
fi


###############################################################################
# CONFIG_OE() - Configure OpenEmbedded
###############################################################################
function config_oe()
{

    MACHINE="${CL_MACHINE}"

    #--------------------------------------------------------------------------
    # Write out the OE bitbake configuration file.
    #--------------------------------------------------------------------------
    mkdir -p ${OE_BUILD_DIR}/conf

    # There's no need to rewrite site.conf when changing MACHINE
    if [ ! -e ${OE_BUILD_DIR}/conf/site.conf ]; then
        cat > ${OE_BUILD_DIR}/conf/site.conf <<_EOF

SCONF_VERSION = "1"

# Where to store sources
DL_DIR = "${OE_SOURCE_DIR}/downloads"

# Where to save shared state
SSTATE_DIR = "${OE_BUILD_DIR}/build/sstate-cache"

# Which files do we want to parse:
BBFILES ?= "${OE_SOURCE_DIR}/openembedded-core/meta/recipes-*/*/*.bb"

TMPDIR = "${OE_BUILD_TMPDIR}"

# Go through the Firewall
#HTTP_PROXY        = "http://${PROXYHOST}:${PROXYPORT}/"

_EOF
fi
    if [ ! -e ${OE_BUILD_DIR}/conf/auto.conf ]; then
        cat > ${OE_BUILD_DIR}/conf/auto.conf <<_EOF
MACHINE ?= "${MACHINE}"
_EOF
    else
	eval "sed -i -e 's/^MACHINE.*$/MACHINE ?= \"${MACHINE}\"/g' ${OE_BUILD_DIR}/conf/auto.conf"
fi
}

###############################################################################
# SET_ENVIRONMENT() - Setup environment variables for OE development
###############################################################################
function set_environment()
{

# Workaround for differences between yocto bitbake and vanilla bitbake
export BBFETCH2=True

export TAG

#--------------------------------------------------------------------------
# If an env already exists, use it, otherwise generate it
#--------------------------------------------------------------------------

if [ -e ${OE_ENV_FILE} ] ; then
    . ${OE_ENV_FILE}
fi

if [ x"${BASE_VERSION}" != x"${SCRIPTS_BASE_VERSION}" ] ; then
	echo "BASE_VERSION mismatch, recreating ${OE_ENV_FILE}"
	rm -f ${OE_ENV_FILE} ${OE_BUILD_DIR}/conf/site.conf
fi

if [ -e ${OE_ENV_FILE} ] ; then
    . ${OE_ENV_FILE}
else

    #--------------------------------------------------------------------------
    # Specify distribution information
    #--------------------------------------------------------------------------
    DISTRO=$(grep -w DISTRO conf/local.conf | grep -v '^#' | awk -F\" '{print $2}')
    DISTRO_DIRNAME=`echo $DISTRO | sed s#[.-]#_#g`

    echo "export SCRIPTS_BASE_VERSION=${BASE_VERSION}" > ${OE_ENV_FILE}
    echo "export BBFETCH2=True" >> ${OE_ENV_FILE}

    echo "export DISTRO=\"${DISTRO}\"" >> ${OE_ENV_FILE}
    echo "export DISTRO_DIRNAME=\"${DISTRO_DIRNAME}\"" >> ${OE_ENV_FILE}

    #--------------------------------------------------------------------------
    # Specify the root directory for your OpenEmbedded development
    #--------------------------------------------------------------------------
    OE_BUILD_DIR=${OE_BASE}
    OE_BUILD_TMPDIR="${OE_BUILD_DIR}/build/tmp-${DISTRO_DIRNAME}"
    OE_SOURCE_DIR=${OE_BASE}/sources
    OE_LAYERS_TXT="${OE_SOURCE_DIR}/layers.txt"

    export BUILDDIR=${OE_BUILD_DIR}
    mkdir -p ${OE_BUILD_DIR}
    mkdir -p ${OE_SOURCE_DIR}
    export OE_BASE

    echo "export OE_BUILD_DIR=\"${OE_BUILD_DIR}\"" >> ${OE_ENV_FILE}
    echo "export BUILDDIR=\"${OE_BUILD_DIR}\"" >> ${OE_ENV_FILE}
    echo "export OE_BUILD_TMPDIR=\"${OE_BUILD_TMPDIR}\"" >> ${OE_ENV_FILE}
    echo "export OE_SOURCE_DIR=\"${OE_SOURCE_DIR}\"" >> ${OE_ENV_FILE}
    echo "export OE_LAYERS_TXT=\"${OE_LAYERS_TXT}\"" >> ${OE_ENV_FILE}

    echo "export OE_BASE=\"${OE_BASE}\"" >> ${OE_ENV_FILE}

    #--------------------------------------------------------------------------
    # Include up-to-date bitbake in our PATH.
    #--------------------------------------------------------------------------
    export PATH=${OE_SOURCE_DIR}/openembedded-core/scripts:${OE_SOURCE_DIR}/bitbake/bin:${PATH}

    echo "export PATH=\"${PATH}\"" >> ${OE_ENV_FILE}

    #--------------------------------------------------------------------------
    # Make sure Bitbake doesn't filter out the following variables from our
    # environment.
    #--------------------------------------------------------------------------
    export BB_ENV_EXTRAWHITE="MACHINE DISTRO TCLIBC TCMODE GIT_PROXY_COMMAND http_proxy ftp_proxy https_proxy all_proxy ALL_PROXY no_proxy SSH_AGENT_PID SSH_AUTH_SOCK BB_SRCREV_POLICY SDKMACHINE BB_NUMBER_THREADS"

    echo "export BB_ENV_EXTRAWHITE=\"${BB_ENV_EXTRAWHITE}\"" >> ${OE_ENV_FILE}

    #--------------------------------------------------------------------------
    # Specify proxy information
    #--------------------------------------------------------------------------
    if [ "x$PROXYHOST" != "x"  ] ; then
        export http_proxy=http://${PROXYHOST}:${PROXYPORT}/
        export ftp_proxy=http://${PROXYHOST}:${PROXYPORT}/

        export SVN_CONFIG_DIR=${OE_BUILD_DIR}/subversion_config
        export GIT_CONFIG_DIR=${OE_BUILD_DIR}/git_config

        echo "export http_proxy=\"${http_proxy}\"" >> ${OE_ENV_FILE}
        echo "export ftp_proxy=\"${ftp_proxy}\"" >> ${OE_ENV_FILE}
        echo "export SVN_CONFIG_DIR=\"${SVN_CONFIG_DIR}\"" >> ${OE_ENV_FILE}
        echo "export GIT_CONFIG_DIR=\"${GIT_CONFIG_DIR}\"" >> ${OE_ENV_FILE}
        echo "export GIT_PROXY_COMMAND=\"\${GIT_CONFIG_DIR}/git-proxy.sh\"" >> ${OE_ENV_FILE}

        config_svn_proxy
        config_git_proxy
    fi

    #--------------------------------------------------------------------------
    # Set up the bitbake path to find the OpenEmbedded recipes.
    #--------------------------------------------------------------------------
    export BBPATH=${OE_BUILD_DIR}:${OE_SOURCE_DIR}/openembedded-core/meta${BBPATH_EXTRA}

    echo "export BBPATH=\"${BBPATH}\"" >> ${OE_ENV_FILE}

    #--------------------------------------------------------------------------
    # Look for dash
    #--------------------------------------------------------------------------
    if [ "$(readlink /bin/sh)" = "dash" ] ; then
	echo "/bin/sh is a symlink to dash, please point it to bash instead"
        exit 1
    fi

    echo "There now is a sourceable script in ${OE_ENV_FILE} You can do '. ${OE_ENV_FILE}' and run 'bitbake something' without using $0 as wrapper"
fi # if -e ${OE_ENV_FILE}

if ! [ -e ${OE_BUILD_DIR}/conf/site.conf ] ; then
	config_oe
fi

}

###############################################################################
# UPDATE_ALL() - Make sure everything is up to date
###############################################################################
function update_all()
{
    set_environment
    update_oe
}

###############################################################################
# CLEAN_OE() - Delete TMPDIR
###############################################################################
function clean_oe()
{
    set_environment
    echo "Cleaning ${OE_BUILD_TMPDIR}"
    rm -rf ${OE_BUILD_TMPDIR}
}


###############################################################################
# OE_BUILD() - Build an OE package or image
###############################################################################
function oe_build()
{
    if [ ! -e ${OE_BUILD_DIR}/conf/auto.conf ] ; then
        if [ -z $MACHINE ] ; then
            echo "No config found, please run $0 config <machine> first"
        else
            CL_MACHINE=$MACHINE
            set_environment
            config_oe && update_all
        fi
    fi

    set_environment
    if [ -e ${OE_ENV_FILE} ] ; then
        echo "Using ${OE_ENV_FILE} to setup needed variables. It is recommended to do '. ${OE_ENV_FILE}' and run 'bitbake something' without using $0 as wrapper"
    fi
    cd ${OE_BUILD_DIR}
    if [ -z $MACHINE ] ; then
        echo "Executing: bitbake" $*
        bitbake $*
    else
        echo "Executing: MACHINE=${MACHINE} bitbake" $*
        MACHINE=${MACHINE} bitbake $*
    fi
}


###############################################################################
# OE_CONFIG() - Configure OE for a target
###############################################################################
function oe_config()
{
    set_environment
    config_oe
    update_all

    echo ""
    echo "Setup for ${CL_MACHINE} completed"
}

###############################################################################
# UPDATE_OE() - Update OpenEmbedded distribution.
###############################################################################
function update_oe()
{
    if [ "x$PROXYHOST" != "x" ] ; then
        config_git_proxy
    fi

    #manage meta-openembedded and meta-angstrom with layerman
    env gawk -v command=update -f ${OE_BASE}/scripts/layers.awk ${OE_LAYERS_TXT}
}

###############################################################################
# CONFIG_SVN_PROXY() - Configure subversion proxy information
###############################################################################
function config_svn_proxy()
{
    if [ ! -f ${SVN_CONFIG_DIR}/servers ]
    then
        mkdir -p ${SVN_CONFIG_DIR}
        cat >> ${SVN_CONFIG_DIR}/servers <<_EOF
[global]
http-proxy-host = ${PROXYHOST}
http-proxy-port = ${PROXYPORT}
_EOF
    fi
}


###############################################################################
# CONFIG_GIT_PROXY() - Configure GIT proxy information
###############################################################################
function config_git_proxy()
{
    if [ ! -f ${GIT_CONFIG_DIR}/git-proxy.sh ]
    then
        mkdir -p ${GIT_CONFIG_DIR}
        cat > ${GIT_CONFIG_DIR}/git-proxy.sh <<_EOF
if [ -x /bin/env ] ; then
    exec /bin/env corkscrew ${PROXYHOST} ${PROXYPORT} \$*
else
    exec /usr/bin/env corkscrew ${PROXYHOST} ${PROXYPORT} \$*
fi
_EOF
        chmod +x ${GIT_CONFIG_DIR}/git-proxy.sh
        export GIT_PROXY_COMMAND=${GIT_CONFIG_DIR}/git-proxy.sh
    fi
}

###############################################################################
# tag_layers - Tag all layers with a given tag
###############################################################################
function tag_layers()
{
    set_environment
    env gawk -v command=tag -v commandarg=$TAG -f ${OE_BASE}/scripts/layers.awk ${OE_LAYERS_TXT}
    echo $TAG >> ${OE_BASE}/tags
}

###############################################################################
# reset_layers - Remove all local changes including stash and ignored files
###############################################################################
function reset_layers()
{
    set_environment
    env gawk -v command=reset -f ${OE_BASE}/scripts/layers.awk ${OE_LAYERS_TXT}
}

###############################################################################
# changelog - Display changelog for all layers with a given tag
###############################################################################
function changelog()
{
	set_environment
	env gawk -v command=changelog -v commandarg=$TAG -f ${OE_BASE}/scripts/layers.awk ${OE_LAYERS_TXT}
}

###############################################################################
# layer_info - Get layer info
###############################################################################
function layer_info()
{
	set_environment
	rm -f ${OE_SOURCE_DIR}/info.txt
	env gawk -v command=info -f ${OE_BASE}/scripts/layers.awk ${OE_LAYERS_TXT}
	echo
	echo "Showing contents of ${OE_SOURCE_DIR}/info.txt:"
	echo
	cat ${OE_SOURCE_DIR}/info.txt
	echo
}

###############################################################################
# checkout - Checkout all layers with a given tag
###############################################################################
function checkout()
{
set_environment
env gawk -v command=checkout -v commandarg=$TAG -f ${OE_BASE}/scripts/layers.awk ${OE_LAYERS_TXT}
}


###############################################################################
# Build the specified OE packages or images.
###############################################################################

# FIXME: convert to case/esac

if [ $# -gt 0 ]
then
    if [ $1 = "update" ]
    then
        update_all
        exit 0
    fi

    if [ $1 = "info" ]
    then
        layer_info
        exit 0
    fi

    if [ $1 = "reset" ]
    then
        reset_layers
        exit 0
    fi

    if [ $1 = "tag" ]
    then
        if [ -n "$2" ] ; then
            TAG="$2"
        else
            TAG="$(date -u +'%Y%m%d-%H%M')"
        fi
        tag_layers $TAG
        exit 0
    fi
    
    if [ $1 = "changelog" ]
    then
        if [ -z $2 ] ; then
            echo "Changelog needs an argument"
            exit 1
        else
            TAG="$2"
        fi
        changelog
        exit 0
    fi
    
    if [ $1 = "checkout" ]
    then
        if [ -z $2 ] ; then
            echo "Checkout needs an argument"
            exit 1
        else
            TAG="$2"
        fi
        checkout
        exit 0
    fi
    if [ $1 = "bitbake" ]
    then
        shift
        oe_build $*
        exit 0
    fi

    if [ $1 = "config" ]
    then
        shift
        CL_MACHINE=$1
        shift
        oe_config $*
        exit 0
    fi

    if [ $1 = "clean" ]
    then
        clean_oe
        exit 0
    fi
fi

# Help Screen
echo ""
echo "Usage: $0 config <machine>"
echo "       $0 update"
echo "       $0 reset"
echo "       $0 tag [tagname]"
echo "       $0 changelog <tagname>"
echo "       $0 checkout <tagname>"
echo "       $0 clean"
echo ""
echo "       Not recommended, but also possible:"
echo "       $0 bitbake <bitbake target>"
echo "       It is recommended to do '. ${OE_ENV_FILE}' and run 'bitbake something' inside ${BUILDDIR} without using oebb.sh as wrapper"
echo ""
echo "You must invoke \"$0 config <machine>\" and then \"$0 update\" prior"
echo "to your first bitbake command"
echo ""
echo "The <machine> argument can be one of the following"
echo "       beagleboard:   BeagleBoard"
echo "       qemuarm        Emulated ARM machine"
echo "       qemumips:      Emulated MIPS machine"
echo "       fri2-noemgd:   Intel FRI2 machine without graphics"
echo ""
echo "Other machines are valid as well, but listing those would make this message way too long"
