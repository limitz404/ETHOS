#!/bin/bash
##########################################################################################
# AUTHORS: TAYLOR DEAN
# FILE: enablePRU.sh
# CREATED: 3 MARCH 2015
# TEAM: ETHOS
# PURPOSE: TO ENABLE THE PRU AND TO LOAD THE DEVICE TREE OVERLAY
# NOTES: THIS FILE SHOULD BE SOURCED IF USE OF ENVIRONMENT VARIABLES IS REQUIRED
##########################################################################################

echo ETHOS-GPIO > /sys/devices/bone_capemgr.9/slots
modprobe uio_pruss
export SLOTS=/sys/devices/bone_capemgr.9/slots
export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins