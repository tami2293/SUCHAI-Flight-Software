#dependencies
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install python3-pip python-pip
sudo apt-get install git-all
sudo apt-get install libsqlite3-dev
sudo apt-get install libzmq3-dev
sudo apt-get install cmake
sudo pip install pyzmq-static
sudo apt install python3-gpiozero python-gpiozero
sudo apt-get install libgps-dev
#sudo pip3 install pyzmq-static
sudo apt-get install python3-zmq
#install libcsp dep
cd ~/Spel/SUCHAI-Flight-Software
cd src/drivers/Linux/libcsp
sudo sh install_csp.sh
cd ~/Spel
#balloon deps
sudo apt-get install gpsd gpsd-clients python-gps #python3-gps
sudo apt-get install python-smbus python3-smbus
sudo apt-get install python-serial python3-serial

#balloon repo
git clone https://github.com/spel-uchile/balloon_experiment.git
#ln -s balloon_experiment/sw/raspberry sub-modules/

#bmp deps
cd ~
git clone https://github.com/adafruit/Adafruit_Python_BMP.git
cd Adafruit_Python_BMP
sudo python setup.py install
sudo python3 setup.py install

#Make mission history directory and submodules symbolic links
cd ~/Spel
mkdir mission-history
ln -s ~/Spel/balloon_experiment/sw/raspberry/ ~/Spel/
mv raspberry sub_modules
cd ~/Spel/SUCHAI-Flight-Software/sandbox/install/
sudo sh install_direwolf.sh
