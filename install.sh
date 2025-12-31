#!/bin/bash

# --- Definicja kolorów i formatowania ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color (Reset)
BOLD='\033[1m'

# Funkcja pomocnicza do nagłówków sekcji
print_step() {
    echo -e "\n${BLUE}${BOLD}--- $1 ---${NC}"
}

set -e

print_step "1. Hardware configuration (I2C)"
if ! grep -q "dtparam=i2c_arm=on" /boot/config.txt; then
    echo -e "${YELLOW}Enabling I2C interface...${NC}"
    sudo raspi-config nonint do_i2c 0
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

print_step "5. Installing app as system service"
if [ -f "../autoapp.service" ]; then
    sudo cp ../autoapp.service /etc/systemd/system/
    sudo systemctl daemon-reload
    sudo systemctl enable autoapp.service
    echo -e "${GREEN}Service has been installed successfully.${NC}"
else
    echo -e "${RED}ERROR: Missing file autoapp.service!${NC}"
    exit 1
fi

echo -e "\n${GREEN}${BOLD}======================================"
echo -e "       Installation finished!         "
echo -e "======================================${NC}"

read -p "$(echo -e ${YELLOW}"Do you want to reboot now? (y/n): "${NC})" choice
case "$choice" in
  y|Y )
    echo -e "${BLUE}Rebooting...${NC}"
    sudo reboot
    ;;
  * )
    echo -e "${YELLOW}Reboot aborted. Remember to restart manually later.${NC}"
    ;;
esac
