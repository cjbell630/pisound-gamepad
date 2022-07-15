#!/bin/bash
screen -X -S gamepad-hostapd quit
sudo pkill hostapd # likes to hang around sometimes for some reason
screen -X -S gamepad-netboot quit
screen -X -S gamepad-wpasupp quit
sudo pkill wpa_supplicant # likes to hang around sometimes for some reason
sudo iw dev WiiUAP del

sleep 3

# create WiiU AP
sudo iw dev wlan0 interface add wlWiiUAP type managed
sudo ip link set wlWiiUAP name WiiUAP

# turn on wpa supplicant
sudo ip link set wlan0 up
sleep 1
cd /home/ubuntu/2pc2drc/drc-hostap/wpa_supplicant
screen -dm -S gamepad-wpasupp sudo ./wpa_supplicant -Dnl80211 -i wlan0 -c ../conf/wpa_supplicant_normal.conf
sleep 1

# set routing TODO change to WiiUAP
sudo ip a a 192.168.1.10/24 dev WiiUAP
sudo ip l set mtu 1800 dev WiiUAP

# turn on netboot
cd /home/ubuntu/2pc2drc/drc-hostap/netboot
screen -dm -S gamepad-netboot sudo ./netboot 192.168.1.255 192.168.1.10 192.168.1.11 40-d2-8a-6a-62-6e

sudo ip link set wlan0 down
sleep 1

cd /home/ubuntu/2pc2drc/drc-hostap/hostapd
while true; do
  # turn on hostapd TODO use -B flag to run in background
  echo "starting hostapd..."
  screen -dm -L -Logfile ./OUT -S gamepad-hostapd sudo ./hostapd -dd ../conf/generated_ap_normal.conf
  echo "" >OUT
  tail -f -n0 ./OUT | grep --line-buffered -q -e "WPA: pairwise key handshake completed (RSN)" -e "disassoc"

  if [ $? == 1 ]; then
    echo "hostapd exited early"
    break
  elif grep -qe "WPA: pairwise key handshake completed (RSN)" OUT; then
    echo "hostapd successfully paired"
    break
  else
    echo "hostapd tried cookie loop"
    screen -X -S gamepad-hostapd quit
    sudo pkill hostapd # likes to hang around sometimes for some reason
    sleep 3
  fi
done
echo "hostapd left running on screen"

# NOTE theoretically this should never be an issue again once hostapd is running, but it would be good to leave this
# loop running in case something happens. but disassoc reason=8 happens both when there's a cringe failure and when
# the gamepad simply turns off soooo

# WPA: pairwise key handshake completed (RSN)
# nl80211: Frame TX command accepted;

# start executable
# cd /home/ubuntu/2pc2drc
# screen -dm -S gamepad-executable sudo ./executable
