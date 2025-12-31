#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'
BOLD='\033[1m'

SERVICE_INSTALLED=false

print_step() {
    echo -e "\n${BLUE}${BOLD}--- $1 ---${NC}"
}


if [ "$EUID" -ne 0 ]; then
    echo -e "${RED}${BOLD}ERROR: Script must be runa with sudo!${NC}"
    exit 1
fi

set -e

print_step "1. Hardware configuration (I2C)"
if ! grep -q "dtparam=i2c_arm=on" /boot/config.txt; then
    echo -e "${YELLOW}Enabling I2C interface...${NC}"
    raspi-config nonint do_i2c 0
    echo -e "${GREEN}I2C interface enabled.${NC}"
else
    echo -e "${GREEN}I2C interface is already enabled.${NC}"
fi

print_step "2. Installing requirements"
if [ -f "./install_requirements.sh" ]; then
    ./install_requirements.sh
else
    echo -e "${RED}ERROR: install_requirements.sh not found!${NC}"
    exit 1
fi

print_step "3. Allow git pull from app"
git config --global --add safe.directory "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

echo -e "${GREEN}Git directory configured.${NC}"

print_step "4. Compiling Autoapp"
cd autoapp
mkdir -p build && cd build
cmake ..
echo -e "${YELLOW}Starting compilation with $(nproc) cores...${NC}"
make -j$(nproc)

read -p "$(echo -e ${YELLOW}"Do you want to install the app as a system service? (y/n): "${NC})" service_choice
case "$service_choice" in
  y|Y )
    if [ -f "../autoapp.service" ]; then
        cp ../autoapp.service /etc/systemd/system/
        systemctl daemon-reload
        systemctl enable autoapp.service
        echo -e "${GREEN}Service has been installed successfully.${NC}"
        SERVICE_INSTALLED=true
    else
        echo -e "${RED}ERROR: Missing file autoapp.service!${NC}"
        exit 1
    fi
    ;;
  * )
    echo -e "${YELLOW}Skipping service installation.${NC}"
    ;;
esac

echo -e "\n${GREEN}${BOLD}======================================"
echo -e "       Installation finished!         "
echo -e "======================================${NC}"

if [ "$SERVICE_INSTALLED" = true ]; then
    read -p "$(echo -e ${YELLOW}"Do you want to reboot now? (y/n): "${NC})" reboot_choice
    case "$reboot_choice" in
      y|Y )
        echo -e "${BLUE}Rebooting...${NC}"
        reboot
        ;;
      * )
        echo -e "${YELLOW}Reboot aborted. Remember to restart manually later.${NC}"
        ;;
    esac
else
    echo -e "${BLUE}No reboot required (service was not installed).${NC}"
fi
