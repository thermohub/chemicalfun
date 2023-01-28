#!/bin/bash
# Installing dependencies needed to build CemicalFun on (k)ubuntu linux 18.04 or macOS

# Temporarily uncomment rows for packages that need to be re-installed
#sudo rm -rf /usr/local/include/nlohmann
#sudo rm -rf /usr/local/include/eigen3/Eigen/Eigen
#sudo rm -rf /usr/local/include/pybind11
#sudo rm -rf /usr/local/include/spdlog
#sudo rm -rf /usr/local/include/fmt

threads=4

# nlohmann/json library v. 3.6.1 or up
# if not installed in /usr/local/include/nlohmann)
test -f /usr/local/include/nlohmann/json.hpp || {

	# Getting nlohmann/json library
	mkdir -p ~/code && \
		cd ~/code && \
		git clone https://github.com/nlohmann/json.git && \
		cd json && \
		mkdir -p build && \
		cd build && \
		cmake .. -DCMAKE_BUILD_TYPE=Release -DJSON_BuildTests=OFF -DJSON_MultipleHeaders=ON && \
		sudo make install

	# Removing generated build files
	cd ~ && \
		 rm -rf ~/code
}

# Eigen3 math library (added for building and installing xGEMS)
# if not installed in /usr/local/include/eigen3)
test -d /usr/local/include/eigen3/Eigen || {

	# Downloading and unpacking eigen3 source code into ~/code/eigen-code
	mkdir -p ~/code && cd ~/code && mkdir eigen-code
	wget -c https://bitbucket.org/eigen/eigen/get/default.tar.bz2
	tar xvjf default.tar.bz2 -C eigen-code --strip-components 1

	# Building Eigen3 library
	cd eigen-code && \
		mkdir -p build && \
		cd build && \
		cmake .. && \
		sudo make install

	# Removing generated build files
	cd ~ && \
		 rm -rf ~/code
}

# fmt 
# if no spdlog installed in /usr/local/lib/ (/usr/local/include/fmt)
test -d /usr/local/include/fmt || {

        # Building thermofun library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://github.com/fmtlib/fmt.git -b 9.1.0  && \
                cd fmt && \
                mkdir -p build && \
                cd build && \
                cmake .. && \
                make && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}

# spdlog 
# if no spdlog installed in /usr/local/lib/ (/usr/local/include/spdlog)
test -d /usr/local/include/spdlog || {

        # Building thermofun library
        mkdir -p ~/code && \
                cd ~/code && \
                git clone https://github.com/gabime/spdlog -b v1.11.0  && \
                cd spdlog && \
                mkdir -p build && \
                cd build && \
                cmake .. -DCMAKE_BUILD_TYPE=Release -DSPDLOG_BUILD_TESTS=OFF -DCMAKE_CXX_FLAGS=-fPIC && \
                make && \
                sudo make install

        # Removing generated build files
        cd ~ && \
                 rm -rf ~/code
}



#Pybind11
test -d /usr/local/include/pybind11 || {

	# Building yaml-cpp library
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

if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
   sudo ldconfig
fi
