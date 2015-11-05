DESCRIPTION = "Daemon to monitor power button"
LICENSE = "GPL"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

SRC_URI = "file://monitor-pwbtn.c \
	file://init \
	file://factory_config_reset.sh \
	file://COPYING \
"

S = "${WORKDIR}"

do_compile() {
	make monitor-pwbtn
}

do_install() {
        install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/monitor-pwbtn ${D}${bindir}
	install -m 0755 ${WORKDIR}/factory_config_reset.sh ${D}${bindir}

        install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/init ${D}${sysconfdir}/init.d/monitor-pwbtn
	update-rc.d -r ${D} monitor-pwbtn start 70 S .
}
