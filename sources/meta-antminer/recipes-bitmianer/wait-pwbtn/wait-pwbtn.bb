DESCRIPTION = "Daemon to wait power button"
LICENSE = "GPL"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

SRC_URI = "file://wait-pwbtn.c \
	file://init \
	file://factory_config_reset.sh \
	file://COPYING \
"

S = "${WORKDIR}"

do_compile() {
	make wait-pwbtn
}

do_install() {
        install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/wait-pwbtn ${D}${bindir}
}
