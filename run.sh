#!/bin/bash
screen -X -S gamepad-hostapd exit
screen -X -S gamepad-netboot exit
screen -X -S gamepad-executable exit

cd /home/ubuntu/2pc2drc/drc-hostap/hostapd
screen -dm -S gamepad-hostapd sudo ./hostapd -dd ../conf/generated_ap_normal.conf
sudo ip a a 192.168.1.10/24 dev wlan0
sudo ip l set mtu 1800 dev wlan0
cd /home/ubuntu/2pc2drc/drc-hostap/netboot
screen -dm -S gamepad-netboot sudo ./netboot 192.168.1.255 192.168.1.10 192.168.1.11 40-d2-8a-6a-62-6e
cd /home/ubuntu/2pc2drc
screen -dm -S gamepad-executable sudo ./executable
