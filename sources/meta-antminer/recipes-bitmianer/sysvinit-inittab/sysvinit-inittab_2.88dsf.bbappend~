FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}-1.0:"

PRINC := "${@int(PRINC) + 1}"

do_install_append() {
echo "dcdc:2345:respawn:${base_sbindir}/monitorcg" >> ${D}${sysconfdir}/inittab
if [ x"S4" == x"${Miner_TYPE}" -o x"S2" == x"${Miner_TYPE}" ]; then
echo "mlcd:2345:respawn:${base_sbindir}/minerlcd" >> ${D}${sysconfdir}/inittab
fi
echo "moni:2345:respawn:${base_sbindir}/miner-m.sh" >> ${D}${sysconfdir}/inittab
}

