do_install_append() {
	update-rc.d -r ${D} dropbear start 40 S .
	sed -i 's%/etc/dropbear/dropbear_rsa_host_key%/config/dropbear_rsa_host_key%g' ${D}${sysconfdir}/init.d/dropbear
	sed -i 's%/etc/dropbear/dropbear_dss_host_key%/config/dropbear_dss_host_key%g' ${D}${sysconfdir}/init.d/dropbear
}
