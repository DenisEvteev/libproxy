#!/bin/bash

module="testmodule"
device="eventshandler"

#unload the module from the kernel
/sbin/rmmod $module $* || exit 1

#remove stale device special file
rm -f /dev/${device}

echo "unload successs"