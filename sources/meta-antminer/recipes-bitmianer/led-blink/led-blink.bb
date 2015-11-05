DESCRIPTION = "Program to blink a led"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

SRC_URI = "file://led-blink.c \
	file://COPYING \
"

S = "${WORKDIR}"

do_compile() {
	make led-blink
}

do_install() {
        install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/led-blink ${D}${bindir}
}
