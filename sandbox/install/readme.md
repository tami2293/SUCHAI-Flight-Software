# Full setup instructions #

## Dependencies ##
* Git

## Install instructions ##
1. mkdir /home/pi/Spel
2. cd ~/Spel
3. git clone https://github.com/spel-uchile/SUCHAI-Flight-Software.git
4. cd SUCHAI-Flight-Software
5. git checkout balloon2
6. cd ~/Spel/SUCHAI-Flight-Software/sandbox/install/
7. sudo sh install_balloon.sh
8. sudo sh install_aprs.sh

## Raspberry setup ##
* Enable i2c, ssh
* Set 3.5 mm jack audio output (raspberry pi 2 and 3)
* Edit the /boot/cmdline.txt
	- Remove console=ttyAMA0,115200 and if there, kgdboc=ttyAMA0,115200
	- Note you might see console=serial0,115200 and should remove those parts of the line if present
* Edit the /boot/config.txt
	- Add this at bottom: enable_uart=1
* Start gpsd
	- sudo killall gpsd
	- (Raspberry pi 2 only) sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock
	- (Raspberry pi 3 and zero) sudo gpsd /dev/ttyS0 -F /var/run/gpsd.sock
* Add this to /etc/default/gpsd
	- (Raspberry pi 2 only) DEVICES="/dev/ttyAMA0 -F /var/run/gpsd.sock"
	- (Raspberry pi 3 and zero) DEVICES="/dev/ttyS0 -F /var/run/gpsd.sock"
* sudo systemctl restart gpsd.socket
* sudo reboot

