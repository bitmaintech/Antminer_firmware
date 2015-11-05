DESCRIPTION = "Daemon to monitor recovery button"
LICENSE = "CLOSED"


SRC_URI = "file://debug.h \
	file://get_mac_ip.c \
	file://get_mac_ip.h \
	file://monitor-ipsig.c \
	file://Makefile \
	file://monitor-ipsig.sh\
"

S = "${WORKDIR}"

do_compile() {
	make 	
}

do_install() {
        install -d ${D}${bindir}
	install -m 0755 ${S}/monitor-ipsig ${D}${bindir}/monitor-ipsig
         
        install -d ${D}${sysconfdir}/init.d
        install -m 0755 ${WORKDIR}/monitor-ipsig.sh ${D}${sysconfdir}/init.d/monitor-ipsig.sh
	update-rc.d -r ${D} monitor-ipsig.sh start 100 S .
}


