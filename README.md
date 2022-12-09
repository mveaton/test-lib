# test-api

  The test-api is a start of a C++ API library (currently built as executable) for common image related code on a BeagleBone AI-64 board (Debian 11.3) and the iPhone (IOS 16). The test-api is still in development and requires error handling, logging and many other features implemented. The repo has been built and tested on the Mac OS (v13.0) with Xcode installed.


# Build Instructions:
  Mac OS
  
    1) Open a terminal and install the following packages using brew
      a) brew install eigen
      b) brew install python
      c) brew install openexr
      d) brew install cmake
      
    2) In the termainl, checkout the repo
      a) cd /your_new_repo_dir
      b) git clone https://github.com/mveaton/test-api.git
      
    3) In the terminal, build the OpenCV library
      a) cd vendor
      b) chmod 755 build_vendor.sh
      b) ./build_vendor.sh
      
    4) In the terminal, build the executable
      a) cd ..
      b) mkdir build
      c) cd build
      d) cmake ..
      e) make
      
# Run Instructions:

    1) In the terminal, run the executable
      a) ./system_API
      
      
      
