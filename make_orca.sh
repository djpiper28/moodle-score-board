#!/bin/bash

git clone https://github.com/cee-studio/orca.git

cd orca
make -j
sudo make install

