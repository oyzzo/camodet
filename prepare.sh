#!/bin/sh

# This script will install all needed dependencies and build OpenCV3
# Use it in a ubuntu style distro or as a guide for other systems

echo "Testing your distro..."
DISTRO=`head -n 1 /etc/lsb-release | cut -d '=' -f 2`

if [ $DISTRO != "Ubuntu" ]; then
	echo "Error: This script is tested in Ubuntu."
	exit 1
fi

# Check script is run with sudo
UID=`id -u`

echo "Checking su...\t\t"
if [ $UID -ne 0 ]; then
	echo "Error: This script must be run with sudo to install pakages."
	exit 1
fi

echo "Installing build tools..."
apt-get update
apt-get install build-essential

echo "Installing required dependencies..."
apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

echo "Installing optional dependencies..."
apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

echo "Getting the OpenCV3 sources..."
git clone https://github.com/opencv/opencv.git opencv

echo "Compiling OpenCV3, This may take a while..."
# Use several threads depending on the cpus of the machine
NCPUS=`cat /proc/cpuinfo | grep processor | wc -l`

cd ./opencv && mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j $NCPUS
make install

# DONE! now everything is prepared to compile camodet
echo "\nDONE, now build camodet:"
echo "1) mkdir build"
echo "2) cd build"
echo "3) cmake .."
echo "4) make"

