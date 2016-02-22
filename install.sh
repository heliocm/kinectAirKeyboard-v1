#!/bin/bash

# Based on - https://www.kdab.com/setting-up-kinect-for-programming-in-linux-part-1/
# http://mitchtech.net/ubuntu-kinect-openni-primesense/
# To operate Kinect we need 2 drivers
# To control LED and tilt motor we use freenect
# http://openkinect.org
# Git: https://github.com/OpenKinect/libfreenect

# To control the video flux and get the depth map we use openNI
# http://openni.org
# Git: https://github.com/OpenNI/OpenNI

# We also nees PrimeSense sensor module ( company that makes Kinect )
# http://www.primesense.com
# Git: https://github.com/avin2/SensorKinect

# Finally, we use a higher level library to get hand tracking, predefined gestures (swipes, push, circles, etc)
# without having to create the algorithms.
# Warning: As this library is not open source and therefore only available as binaries
# you only would be able to install/use it on Windows or Ubuntu.
# http://www.openni.org/Downloads/OpenNIModules.aspx
# choose “OpenNI compliant Middleware Binaries” – “stable” then select your OS

mkdir KinectLibs
cd KinectLibs
git clone https://github.com/OpenKinect/libfreenect
git clone https://github.com/OpenNI/OpenNI
git clone https://github.com/avin2/SensorKinect

sudo apt-get install cmake libglut3-dev pkg-config build-essential libxmu-dev libxi-dev libusb-1.0-0-dev python

# if you get “Unable to locate package libglut3-dev”, use this command instead:

sudo apt-get install cmake freeglut3-dev pkg-config build-essential libxmu-dev libxi-dev libusb-1.0-0-dev python

sudo add-apt-repository "deb http://archive.canonical.com/ lucid partner"
sudo apt-get update
sudo apt-get install sun-java6-jdk

sudo apt-get install doxygen mono-complete graphviz


# Install openKinect (libFreenect)

# in libfreenect directory, in the KinectLibs dir
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig /usr/local/lib64/

 

# Once libFreenect is installed, plug the Kinect, then set permission to R/W on the usb devices (motor and camera).

sudo chmod a+rw /dev/bus/usb//
sudo chmod a+rw /dev/bus/usb//

lsusb | grep Xbox

#libusb couldn't open USB device /dev/bus/usb/001/006: Permission denied.
#libusb requires write access to USB device nodes.




