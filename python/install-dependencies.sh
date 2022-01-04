#!/bin/bash
# Installing dependencies needed to build ChemicalFun python wraper

#sudo rm -r /usr/local/include/pybind11

# Installing pybind11
test -d /usr/local/include/pybind11 || {

	# Building velocypack library
	mkdir -p ~/code && \
		cd ~/code && \
		git clone https://github.com/pybind/pybind11.git && \
		cd pybind11 && \
		mkdir -p build && \
		cd build && \
		cmake .. -DPYBIND11_TEST=OFF && \
		make && \
		sudo make install

	# Removing generated build files
	cd ~ && \
		 rm -rf ~/code
}

sudo ldconfig
