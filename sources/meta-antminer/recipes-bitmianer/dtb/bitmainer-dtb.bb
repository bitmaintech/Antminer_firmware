DESCRIPTION = "Device tree binary for Beagle bone in bitmainers"
LICENSE = "unknown"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"
PR = "r0"
SRC_URI = "file://am335x-boneblack.dts \
	file://am335x-bone-common.dtsi \
	file://am335x-bone-btm.dtsi \
	file://am33xx.dtsi \
	file://skeleton.dtsi \
	file://tps65217.dtsi \
	file://COPYING"

S = "${WORKDIR}"

do_compile() {
	dtc am335x-boneblack.dts -O dtb -o am335x-boneblack-bitmainer.dtb
}

do_install() {
	cp -p *.dtb ${DEPLOY_DIR_IMAGE}/.
}
