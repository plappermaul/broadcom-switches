#!/bin/sh
# $Id: $
# $Copyright: (c) 2016 Broadcom.
# Broadcom Proprietary and Confidential. All rights reserved.$

# Script to Load BCM Kernel modules and start user shell

mknod /dev/linux-bcm-net c 123 0
mknod /dev/linux-bcm-diag c 124 0
ln -sf /dev/linux-bcm-diag /dev/linux-bcm-diag-full
mknod /dev/linux-uk-proxy c 125 0
mknod /dev/linux-bcm-core c 126 0
mknod /dev/linux-kernel-bde c 127 0
ln -sf  /dev/linux-bcm-core  /dev/linux-user-bde

insmod ./linux-kernel-bde.ko
insmod ./linux-user-bde.ko
#insmod ./linux-bcm-core.ko
#insmod ./linux-bcm-diag-full.ko
#insmod ./linux-bcm-diag.ko
#insmod ./linux-bcm-net.ko
#insmod ./linux-uk-proxy.ko

./bcm.user
