#!/bin/bash

CURDIR=$(pwd)

# Install wiring pi
cd /tmp
git clone https://github.com/WiringPi/WiringPi.git
cd /tmp/WiringPi
./build

# Compile PSCTRL
cd $CURDIR
make clean
make all

# Install PSCTRL
sudo systemctl stop psctrl.service
sudo cp psctrl /opt/
sudo chown root:root /opt/psctrl
sudo chmod 0755 /opt/psctrl

# Run PSCTRL at startup
sudo cp psctrl.service /lib/systemd/system/
sudo chown root:root /lib/systemd/system/psctrl.service
sudo chmod 644 /lib/systemd/system/psctrl.service
sudo systemctl daemon-reload
sudo systemctl start psctrl.service
sudo systemctl enable psctrl.service
