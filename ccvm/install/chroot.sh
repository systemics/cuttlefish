#!/bin/sh

# setup console terminal
#
cd /mnt/etc
curl -O somedomain.com/arch/file/vconsole.conf

# setup locales
#
cd /mnt/etc
curl -O somedomain.com/arch/file/locale.gen
curl -O somedomain.com/arch/file/locale.conf
locale-gen

# setup timezone
#
ln -s /usr/share/zoneinfo/America/Los_Angeles /etc/localtime

# set system clock to hardware clock and hardware clock to UTC
#
hwclock --systohc --utc

# set hostname (TBD make this an argument to this script)
#
echo ccvm > /etc/hostname

# enable DHCP client on reboot
#
systemctl enable dhcpcd.service

# get latest package database
#
yes | pacman -Sy

# set root password (TBD make this an argument to this script)
#
echo foambat foambat | tr ' ' '\n' | passwd

# install and configure boot loader
#
yes | pacman -S syslinux
syslinux-install_update -i -a -m
cd /boot/syslinux
curl -O somedomain.com/arch/file/syslinux.cfg

# install virtualbox modules
#
yes | pacman -S virtualbox-guest-modules
cd /etc/modules-load.d
curl -O somedomain.com/arch/file/virtualbox.conf
#yes | pacman -S virtualbox-guest-utils

# install and start sshd with root login enabled
#
yes | pacman -S openssh
cd /etc/ssh
curl -O somedomain.com/arch/file/sshd_config
systemctl start sshd
systemctl enable sshd.service

# do other stuff later (maybe) like install puppet
#
yes | pacman -S ruby
yes | pacman -S lsb-release
gem install puppet facter --no-ri --no-rdoc --no-user-install -V
cp `gem contents puppet | grep puppet.service` /usr/lib/systemd/system

# done with chroot script
#
exit
