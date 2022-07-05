# TODO make into makefile
cd /home/ubuntu/2pc2drc/pisound-gamepad
gcc -c -o src/okayu.o src/okayu.c
g++ -g -c src/GamepadScreen.cpp -o src/GamepadScreen.o
g++ -g src/main.cpp src/*.o src/*.h -o ../executable -l drc -lpthread -lx264 -lm -ldl -lswscale