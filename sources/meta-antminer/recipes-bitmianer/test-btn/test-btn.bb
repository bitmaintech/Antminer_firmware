DESCRIPTION = "Daemon to monitor recovery button"
LICENSE = "GPL"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

SRC_URI = "file://test-key.c \
	file://init_test \
	file://test.sh \
	file://test-hash.c \
	file://test-hash.h \
	file://COPYING \
"

S = "${WORKDIR}"

do_compile() {
	make test-key
	make test-hash	
	#make monitor-pwbtn
}

do_install() {
    install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/test-key ${D}${bindir}/test-btn
	install -m 0755 ${WORKDIR}/test-hash ${D}${bindir}
	install -m 0755 ${WORKDIR}/test.sh ${D}${bindir}

    install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/init_test ${D}${sysconfdir}/init.d/test-btn.sh
	update-rc.d -r ${D} test-btn.sh start 95 S .
}
