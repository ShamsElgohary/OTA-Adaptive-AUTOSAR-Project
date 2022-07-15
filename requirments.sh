
#!/bin/sh

sudo apt-get install libssl-dev
sudo ldconfig -v
git clone https://github.com/whoshuu/cpr.git
cd cpr
mkdir build && cd build
cmake ..
make
sudo make install

sudo apt-get install libboost-all-dev

sudo apt-get install nlohmann-json3-dev