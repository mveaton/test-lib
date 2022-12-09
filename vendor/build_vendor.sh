#!/bin/bash

# Determine CPU architecture
arch=$(uname -m)
if [[ $arch == "x86_64" ]]; then
  echo "x86_64 Architecture"
elif [[ "$arch" == "x86" ]]; then
  echo "X32 Architecture"
elif [[ "$arch" == "arm" ]]; then
  echo "ARM32 Architecture"
elif [[ "$arch" == "aarch64" ]]; then
  echo "ARM64 Architecture"
else
  echo "*** Unknown Architecture ***"
fi

# Get remote library repos to build
git submodule update --init --recursive

cd src

# Build opencv and opencv_contrib
cd opencv
rm -rf build
mkdir build && cd build

cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D OPENCV_EXTRA_MODULES_PATH= ../../opencv_contrib/modules \
      -D WITH_GSTREAMER=ON \
      -D BUILD_ZLIB=OFF \
      -D BUILD_SHARED_LIBS=OFF \
      -D OPENCV_GENERATE_PKGCONFIG=ON \
      -D OPENCV_GENERATE_PKGCONFIG=YES \
      -D CMAKE_INSTALL_PREFIX=../../../install/opencv_${arch} ..

make -j2
make install
cd ../..

# Build OpenCV for IOS
#xcode-select --install

python3 opencv/platforms/apple/build_xcframework.py \
--out ./opencv-build \
--contrib opencv_contrib \
--iphoneos_archs arm64 \
--iphonesimulator_archs x86_64 \
--iphoneos_deployment_target 11.0 \
--build_only_specified_archs

cd opencv-build/iphonesimulator/build/build-x86_64-iphonesimulator/modules/objc_bindings_generator/ios/gen
