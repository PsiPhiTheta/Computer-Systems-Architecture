#!/bin/sh
#Author: 9563426
#Subject: Computer Systems Architecture

cat /proc/cpuinfo #Processor information

cat /proc/device-tree/model #Model information (only available on recent raspi distro)

sudo apt-get install htop #Install special procmon package

htop #CPU/GPU/RAM usage

free -h -s 1 #RAM usage (updates every second)

cat /etc/debian_version #Debian version information

uname -a #Kernal version information