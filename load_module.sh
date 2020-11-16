#!/bin/bash

module="testmodule" #name of device itself
device="eventshandler" #name of device special file or node in file system
mode="644"

#invoke insmode with all argument given to the script
#newer modutils don't look in . by default so use pathname
/sbin/insmod ./$module.ko $* || exit 1

#remove previous instance of device special file with the same name
rm -f /dev/${device}
#acquire major number generated dynamically while
#loading our module in the kernel

major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)
#echo $major
mknod /dev/${device} c $major 0

group="staff"
grep -q staff /etc/group || group="wheel"

#echo $group
chgrp $group /dev/${device}
chmod $mode /dev/${device}
echo "load success"