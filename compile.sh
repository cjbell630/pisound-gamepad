# TODO make into makefile
if (( EUID != 0 )); then
  echo "This script should be run with sudo!!!"
  exit
fi

cd /home/ubuntu/2pc2drc/pisound-gamepad
gcc -c -o src/okayu.o src/okayu.c
g++ -g -c src/GamepadScreen.cpp -o src/GamepadScreen.o
g++ -g -c src/GamepadInput.cpp -o src/GamepadInput.o -l drc
g++ -g -c src/Character.cpp -o src/Character.o -l drc
g++ -g -c src/main.cpp -o src/main.o -l drc -lpthread -lx264 -lm -ldl -lswscale
g++ -g src/*.o src/*.h -o ../executable -l drc -lpthread -lx264 -lm -ldl -lswscale