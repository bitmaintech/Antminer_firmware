DESCRIPTION = "Initc application for production images for bitmainers"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"
#file://spitop_noncerev_c1.rbf 
SRC_URI = "file://initc \
	file://spitop_noncerev.rbf \
	file://spitop_noncerev_c1.rbf \
	file://spitop_noncerev_s5.rbf \
	file://spitop_noncerev_s5_test.rbf \
	file://spitop_noncerev_s2.rbf \
	file://spitop_noncerev_s4p.rbf \
	file://initc.sh \
	file://initc_emmc_flash.sh \
	file://eeprom.dump \
        file://spi-test \
        file://asic_test \
        file://inita \
        file://waas\
	file://COPYING"

S = "${WORKDIR}"
#Miner_TYPE = "C1"
do_install() {
        install -d ${D}${bindir}
        install -m 0755 ${S}/initc ${D}${bindir}
        if [ x"C1" == x"${Miner_TYPE}" ]; then
		install -m 0644 ${S}/spitop_noncerev_c1.rbf ${D}${bindir}/spitop_noncerev.rbf #fpga bin file
		echo "C1"
	elif [ x"C2" == x"${Miner_TYPE}" ]; then
            	install -m 0644 ${S}/spitop_noncerev_c1.rbf ${D}${bindir}/spitop_noncerev.rbf #fpga bin file
            	echo "C2"
	elif [ x"S5" == x"${Miner_TYPE}" ]; then
            	install -m 0644 ${S}/spitop_noncerev_s5.rbf ${D}${bindir}/spitop_noncerev.rbf #fpga bin file
            	echo "S5"
	elif [ x"S5+" == x"${Miner_TYPE}" ]; then
            	install -m 0644 ${S}/spitop_noncerev_s5_test.rbf ${D}${bindir}/spitop_noncerev.rbf #fpga bin file
            	echo "S5+"

	elif [ x"S2" == x"${Miner_TYPE}" ]; then
            	install -m 0644 ${S}/spitop_noncerev_s2.rbf ${D}${bindir}/spitop_noncerev.rbf #fpga bin file
            	echo "S2"
	elif [ x"S4+" == x"${Miner_TYPE}" ]; then
            	install -m 0644 ${S}/spitop_noncerev_s4p.rbf ${D}${bindir}/spitop_noncerev.rbf #fpga bin file
            	echo "S4+"
	else
		install -m 0644 ${S}/spitop_noncerev.rbf ${D}${bindir} #fpga bin file
		echo "S4"	
	fi
	install -m 0755 ${S}/spi-test ${D}${bindir}
        install -m 0755 ${S}/asic_test ${D}${bindir}
        install -m 0755 ${S}/inita ${D}${bindir}
	install -m 0755 ${S}/waas ${D}${bindir}

	install -d ${D}${sysconfdir}
	install -m 0400 ${WORKDIR}/eeprom.dump ${D}${sysconfdir}/eeprom.dump
        install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/initc.sh ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/initc_emmc_flash.sh ${D}${sysconfdir}/init.d
#cd ${D}${sysconfdir}/init.d
#		rm -rf initc.sh
#		mv initc_emmc_flash.sh initc.sh
	update-rc.d -r ${D} initc.sh start 36 S .
}
