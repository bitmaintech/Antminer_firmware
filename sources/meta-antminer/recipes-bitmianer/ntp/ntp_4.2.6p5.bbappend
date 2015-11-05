FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-1.0:"

PRINC := "${@int(PRINC) + 1}"

do_install_append() {
	sed -i 's!time.server.example.com!pool.ntp.org!g' ${D}${sysconfdir}/ntp.conf
}
