DESCRIPTION = "Daemon to monitor recovery button"
LICENSE = "GPL"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

SRC_URI = "file://monitor-recobtn.c \
	file://monitor-recobtn-c1.c \
	file://init \
	file://init-c1 \
	file://factory_config_reset.sh \
	file://COPYING \
"

S = "${WORKDIR}"

do_compile() {
	make monitor-recobtn
	make monitor-recobtn-c1
}

do_install() {
    install -d ${D}${bindir}
	if [ x"C1" == x"${Miner_TYPE}" ]; then
	install -m 0755 ${WORKDIR}/monitor-recobtn-c1 ${D}${bindir}/monitor-recobtn
	else
	install -m 0755 ${WORKDIR}/monitor-recobtn ${D}${bindir}/monitor-recobtn
	fi
	install -m 0755 ${WORKDIR}/factory_config_reset.sh ${D}${bindir}

    install -d ${D}${sysconfdir}/init.d
	if [ x"C1" == x"${Miner_TYPE}" ]; then
	install -m 0755 ${WORKDIR}/init-c1 ${D}${sysconfdir}/init.d/monitor-recobtn
	else
	install -m 0755 ${WORKDIR}/init ${D}${sysconfdir}/init.d/monitor-recobtn
	fi
	update-rc.d -r ${D} monitor-recobtn start 70 S .
}
