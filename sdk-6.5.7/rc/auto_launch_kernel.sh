#!/bin/sh

# $Id: auto_launch_kernel.sh,v 1.7 Broadcom SDK $
# $Copyright: (c) 2016 Broadcom.
# Broadcom Proprietary and Confidential. All rights reserved.$

# mount fatfs to make config.bcm available
grep -q BCM9562 /proc/cpuinfo
if [ $? == 0 ] ; then
    mount -t vfat /dev/mtdblock0 /mnt
else
    grep -q BCM5300 /proc/cpuinfo
    if [ $? == 0 ] ; then
        mount -t vfat /dev/mtdblock2 /mnt
    else
        grep -q BCM98548 /proc/cpuinfo
        if [ $? == 0 ] ; then
            grep -q "Linux version 2.6" /proc/version
            if [ $? == 0 ] ; then
                mount -t vfat /dev/mtdblock1 /mnt
            fi
        fi
    fi
fi

cd /broadcom
insmod ./linux-kernel-bde.ko
insmod ./linux-uk-proxy.ko
insmod ./linux-bcm-diag-full.ko
if [ -f /mnt/config.bcm ] ; then
   export BCM_CONFIG_FILE=/mnt/config.bcm
fi
./bcm.user.proxy
