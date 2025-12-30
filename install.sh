#!/bin/bash

set -e

echo "--- 1. Hardware configuration (I2C) ---"
if ! grep -q "dtparam=i2c_arm=on" /boot/config.txt; then
    echo "Enabling I2C interface..."
    sudo raspi-config nonint do_i2c 0
    echo "I2C interface enabled."
else
    echo "I2C interface enabled yet."
fi

echo "--- 2. Installing requirements ---"
./install_requirements.sh

echo "--- 3. Allow git pull from app ---"
git config --global --add safe.directory "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

echo "--- 4. Compiling Autoapp ---"
cd autoapp
mkdir -p build && cd build
cmake ..
make -j$(nproc)

echo "--- 5. Installing app as system service ---"
if [ -f "autoapp.service" ]; then
    sudo cp autoapp.service /etc/systemd/system/
    sudo systemctl daemon-reload
    sudo systemctl enable autoapp.service
    echo "Service has been installed successfully."
else
    echo "ERROR: Missing file autoapp.service w autoapp/build!"
    exit 1
fi

echo "--- Installation finished ---"
read -p "Do you want to reboot now? (y/n): " choice
case "$choice" in
  y|Y ) sudo reboot ;;
  * ) echo "Reboot abort." ;;
esac
