sudo apt install -y \
g++ cmake make \
protobuf-compiler libprotobuf-dev libboost-system-dev libboost-log-dev libusb-1.0-0-dev \
qt6-base-dev qt6-multimedia-dev qt6-declarative-dev qt6-connectivity-dev qt6-serialbus-dev qt6-wayland \
qml6-module-qtquick-controls qml6-module-qtquick qml6-module-qtqml-workerscript qml6-module-qtquick-templates qml6-module-qtquick-layouts qml6-module-qtmultimedia qml6-module-qtquick-window qml6-module-qtquick-shapes
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/bin sh
arduino-cli core install arduino:avr
arduino-cli lib install Streaming Vector Keyboard
