#!/bin/sh

echo "flash MLO"
flash_eraseall /dev/mtd0
nandwrite -p /dev/mtd0 /mnt/mmc1/MLO
sh_eraseall /dev/mtd1
nandwrite -p /dev/mtd1 /mnt/mmc1/MLO
flash_eraseall /dev/mtd2
nandwrite -p /dev/mtd2 /mnt/mmc1/MLO
flash_eraseall /dev/mtd3
nandwrite -p /dev/mtd3 /mnt/mmc1/MLO

echo "flash u-boot"
flash_eraseall /dev/mtd4
nandwrite -p /dev/mtd4 /mnt/mmc1/u-boot.img

echo "flash dtb"
flash_eraseall /dev/mtd6
nandwrite -p /dev/mtd6 /mnt/mmc1/am335x-boneblack-bitmainer.dtb

echo "flash kernel"
flash_eraseall /dev/mtd7
nandwrite -p /dev/mtd7 /mnt/mmc1/uImage.bin

echo "flash romfs"
flash_eraseall /dev/mtd8
nandwrite -p /dev/mtd8 /mnt/mmc1/initramfs.bin.SD

flash_eraseall -j /dev/mtd9

sync

