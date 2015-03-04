#!/bin/bash
##########################################################################################
# AUTHORS: PATRICK KLEIN, TAYLOR DEAN
# FILE: reflashScript.sh
# CREATED: 3 MARCH 2015
# TEAM: ETHOS
# PURPOSE: TO PERFORM REQUIRED TASKS AFTER REFLASH
# NOTES: THIS FILE MUST BE RUN FROM WITHIN A DIRECORY WITH REQUIRED FILES
##########################################################################################


echo "Copying files to /root/..."
cp -r * /root/

echo "Setting permissions..."
#cd /root/
chmod -R 777 /root/

export CROSS_COMPILE=

echo "Compiling interface..."
cd /root/am335x_pru_package-master/pru_sw/app_loader/interface
make clean
make

echo "Copying pasm to pasm_linuxintel..."
cd /root/am335x_pru_package-master/pru_sw/utils
mv pasm pasm_linuxintel

echo "Running linuxbuild..."
cd /root/am335x_pru_package-master/pru_sw/utils/pasm_source
source ./linuxbuild

#echo "Compiling example_apps..."
#cd /root/am335x_pru_package-master/pru_sw/example_apps
#make clean
#make

echo "Copying library files..."
cd /root/am335x_pru_package-master/pru_sw/app_loader/lib
cp * /usr/lib

echo "Copying header files..."
cd /root/am335x_pru_package-master/pru_sw/app_loader/include
cp * /usr/include

echo "Copying pasm..."
cd /root/am335x_pru_package-master/pru_sw/utils/
cp pasm /usr/bin

echo "Copying device tree..."
cp ETHOS-GPIO-00A0.dts /lib/firmware

echo "Converting device tree file..."
cd /lib/firmware/
dtc -@ -O dtb -o ETHOS-GPIO-00A0.dtbo ETHOS-GPIO-00A0.dts

echo "DONE!"
echo "Reboot, disable HDMI, reboot, source enablePRU.sh, and recompile code!"