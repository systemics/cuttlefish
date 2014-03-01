#!/bin/sh

# test that we're online
#
if ! ping -c 1 -w 5 "www.google.com" &>/dev/null ; then 
  echo "not online. fix the network"
  exit
fi

# partition the disk
#
echo o.n.p.1...w. | tr '.' '\n' | fdisk /dev/sda

# format the disk partition
#
mkfs.ext4 /dev/sda1

# mount the target disk partition
#
mount /dev/sda1 /mnt

# install the base Arch system
#
pacstrap /mnt base

# grab our chroot script and make it executable
#
cd /mnt/root
curl -O somedomain.com/arch/chroot.sh
chmod 750 chroot.sh

# run our chroot script
#
arch-chroot /mnt /root/chroot.sh

# call it good and exit
#
umount -R /mnt
reboot
