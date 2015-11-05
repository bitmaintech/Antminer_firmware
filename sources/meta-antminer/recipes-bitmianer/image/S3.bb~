export IMAGE_BASENAME = "S3"
IMAGE_INSTALL = " \
	busybox \
	base-files \
	base-passwd \
	initscripts \
	sysvinit \
	sysvinit-pidof \
	angstrom-version \
	tinylogin \
	i2c-tools \
	screen \
	dropbear \
	libcurl \
	lighttpd \
	dtc \
	stunnel \
	initc-bin \
	openssl \
	mtd-utils \
	curl \
	udev \
	monitor-recobtn \
	test-btn \
	ntp \
	ntpdate \
	netbase \
	avahi \
	led-blink \
	cgminer \
	bitmain-spi-s3 \
	bitmain-lcd \
"
#omap3-mkcard 			   
#cgminer 

inherit image

do_rootfs_append() {
	echo "do_build_append"
	#cp -rf /home/xxl/AM335x/knc/setup-s3/deploy/eglibc/images/beaglebone/Angstrom-S3-eglibc-ipk-v2013.06-beaglebone.rootfs.cpio.gz.u-boot /tftpboot/initramfs.bin
	#cp -rf /home/xxl/AM335x/knc/setup-s3/deploy/eglibc/images/beaglebone/Angstrom-S3-eglibc-ipk-v2013.06-beaglebone.rootfs.cpio.gz.u-boot /tftpboot/initramfs.bin.SD
}

