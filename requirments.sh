
#!/bin/sh

sudo apt-get install libboost-all-dev

sudo apt-get install nlohmann-json3-dev

sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils

cd ..
sudo apt-get install libssl-dev
sudo ldconfig -v
git clone https://github.com/whoshuu/cpr.git
cd cpr
mkdir build && cd build
cmake ..
make
sudo make install
