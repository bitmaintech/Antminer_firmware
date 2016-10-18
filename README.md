meta-bitmainer
=============

OpenEmbedded / Ångström meta layer for bitminer Miner

Setup instructoins
------------------

To build a bitmainer miner firmware, first prepare a build machine as per OpenEmbedded Required software instructions at http://www.openembedded.org/wiki/Getting_started

When machine is prepared proceed to prepare the bitmainer firmware build by running

    git clone git@github.com:bitmaintech/Antminer_firmware.git
    cd antminer_firmware
    MACHINE=beaglebone ./oebb.sh config beaglebone

Rootfs build instructions
----------------------------------

To build the firmware rootfs run

    . environment-angstrom-v2013.06
    bitbake bitmainer

and then wait for quite a bit for the build to complete.

Note: You will need ~11GB free disk space.

The resulting root initramfs will be in

    antminer_firmware/deploy/eglibc/images/beaglebone/Angstrom-bitmainer-eglibc-ipk-v2013.06-beaglebone.rootfs.cpio.gz.u-boot


Kernel build instructions
-----------------------------------

To build the Linux kernel image run

    . environment-angstrom-v2013.06
    bitbake virtual/kernel

which results in a kernel image at

    antminer_firmware/deploy/eglibc/images/beaglebone/
