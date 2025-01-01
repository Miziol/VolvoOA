# OpenAuto Desktop Head Unit

### Build for Raspberry Pi 3

1. install packages for build

       # packages for aasdk
       sudo apt install -y libboost-all-dev libusb-1.0-0-dev libssl-dev cmake libprotobuf-dev protobuf-c-compiler protobuf-compiler
       # packages for openauto
       sudo apt install -y libqt5multimedia5 libqt5multimedia5-plugins libqt5multimediawidgets5 qtmultimedia5-dev libqt5bluetooth5 libqt5bluetooth5-bin qtconnectivity5-dev pulseaudio librtaudio-dev

2. compile ilclient

       sudo apt install -y libraspberrypi-doc libraspberrypi-dev
       cd /opt/vc/src/hello_pi/libs/ilclient
       make

[comment]: # (
       sudo apt install -y subversion
       svn checkout https://github.com/raspberrypi/firmware/trunk/opt
       export SDKSTAGE=`pwd`
       cd opt/vc/src/hello_pi/libs/ilclient
       make
       )
   
3. build

       # clone this repository
       git clone --recursive https://github.com/imchos/openauto-dhu.git

       # build this repository
       cd openauto-dhu
       mkdir build; cd build
       cmake -DCMAKE_BUILD_TYPE=Release -DRPI3_BUILD=TRUE ..
       make -j2
       cd ..

4. add udev rules

       sudo echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="*", ATTR{idProduct}=="*", MODE="0660", GROUP="plugdev"' | sudo tee /etc/udev/rules.d/99-openauto.rules
       sudo udevadm control --reload-rules && sudo udevadm trigger

5. run

       ./openauto/bin/autoapp
