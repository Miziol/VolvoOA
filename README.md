# VolvoOA

VolvoOA is a project which extend OpenAuto project with car integration parts.

### Features

1. Read coolant temperature using OBD II standard
2. Display it on DIM of the car and/or inside AutoApp
3. Read button click from LIN bus
4. Emulate keyboard for AA or use Serial communication

### Outside part of project

- AAsdk and Openauto forks from [imchos](https://github.com/imchos/openauto-dhu)

## Install

### Tools

sudo apt install git g++ cmake make protobuf-compiler

### Libs

sudo apt install libboost-all-dev libssl-dev qt6-multimedia-dev qt6-base-dev qt6-declarative-dev qt6-connectivity-dev libusb-1.0-0-dev qml6-module-qtquick qml6-module-qtquick-controls qml6-module-qtqml-workerscript qml6-module-qtquick-templates qml6-module-qtmultimedia

## Build

cd autoapp
mkdir build
cd build
cmake ..
make
