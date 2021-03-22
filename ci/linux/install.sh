#!/bin/bash

  # Install apt-get for qt 5.11.2
sudo add-apt-repository -y ppa:beineri/opt-qt-5.11.2-xenial
  # For g++
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
  # Update
sudo apt-get update -qq
  # Install qt
sudo apt-get install -y qt511-meta-minimal
#sudo apt-get install -y qt511base
#sudo apt-get install -y qt5-dev-tools

# setup qt
source /opt/qt511/bin/qt511-env.sh
export QTHOME=/opt/qt511

  # Install g++
sudo apt-get install -y g++-6

  # Some random opengl related stuff adviced by a chinese
sudo apt install -y mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev

  # Install doxy + graphviz
sudo apt-get install -y doxygen
sudo apt-get install -y doxygen-doc
sudo apt-get install -y doxygen-latex
sudo apt-get install -y doxygen-gui
sudo apt-get install -y graphviz
sudo apt-get install -y pandoc
