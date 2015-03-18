#!/bin/bash
##########################################################################################
# AUTHORS: PATRICK KLEIN, TAYLOR DEAN
# FILE: reflashScript.sh
# CREATED: 3 MARCH 2015
# TEAM: ETHOS
# PURPOSE: TO PERFORM REQUIRED TASKS AFTER REFLASH
# NOTES: THIS FILE MUST BE RUN FROM WITHIN A DIRECORY WITH REQUIRED FILES
##########################################################################################

/bin/echo "Setting permissions..."
/bin/chmod -R 777 /root/

export CROSS_COMPILE=

/bin/echo "Compiling interface..."
cd /root/ethosSoftware/requiredFiles/am335x_pru_package-master/pru_sw/app_loader/interface
/usr/bin/make clean
/usr/bin/make

/bin/echo "Copying pasm to pasm_linuxintel..."
cd /root/ethosSoftware/requiredFiles/am335x_pru_package-master/pru_sw/utils
/bin/mv pasm pasm_linuxintel

/bin/echo "Running linuxbuild..."
cd /root/ethosSoftware/requiredFiles/am335x_pru_package-master/pru_sw/utils/pasm_source
source ./linuxbuild

/bin/echo "Copying library files..."
cd /root/ethosSoftware/requiredFiles/am335x_pru_package-master/pru_sw/app_loader/lib
/bin/cp * /usr/lib/

/bin/echo "Copying header files..."
cd /root/ethosSoftware/requiredFiles/am335x_pru_package-master/pru_sw/app_loader/include
/bin/cp * /usr/include/

/bin/echo "Copying pasm..."
cd /root/ethosSoftware/requiredFiles/am335x_pru_package-master/pru_sw/utils/
/bin/cp pasm /usr/bin/

/bin/echo "Copying device tree..."
/bin/cp /root/ethosSoftware/requiredFiles/ETHOS-GPIO-00A0.dts /lib/firmware/

/bin/echo "Converting device tree file..."
cd /lib/firmware/
/usr/bin/dtc -@ -O dtb -o ETHOS-GPIO-00A0.dtbo ETHOS-GPIO-00A0.dts

/bin/echo "Turning off HDMI..."
/bin/cp /root/ethosSoftware/requiredFiles/uEnv.txt /boot/uboot/uEnv.txt

/bin/echo "Making ETHOS software..."

/bin/echo "DONE!"
/bin/echo "Reboot, source enablePRU.sh, run code!"