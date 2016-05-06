cd build
cmake ..
make
rm -rf CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile
mv * ..
cd ..