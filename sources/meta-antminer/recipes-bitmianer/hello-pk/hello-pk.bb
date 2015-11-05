LICENSE = "CLOSED"

SRC_URI = "file://* \
        "

S = "${WORKDIR}"

do_compile() {
        make
}
do_install() {
        install -d ${D}${bindir}
        install -m 0755 ${S}/hello ${D}${bindir}/
}

#LICENSE = "GPL"
#LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"
PACKAGES = "${PN}"
