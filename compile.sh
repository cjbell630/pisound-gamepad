# TODO make into makefile
if ((EUID != 0)); then
  echo "This script should be run with sudo!!!"
  exit
fi

make_c() {
  base_name="${1%.*}"
  if [ ! -e "$base_name.bck" ]; then touch "$base_name.bck"; fi
  if cmp -s "$base_name.c" "$base_name.bck"; then
    cmp "$base_name.c" "$base_name.bck"
    echo "No changes to $base_name.c..."
  else
    echo "Making $base_name.o..."
    gcc -c -o "$base_name.o" "$base_name.c" "$2"
    cp "$base_name.c" "$base_name.bck"
  fi
}

make_cpp() {
  base_name="${1%.*}"
  if [ ! -e "$base_name.bck" ]; then touch "$base_name.bck"; fi
  if cmp -s "$base_name.cpp" "$base_name.bck"; then
    cmp "$base_name.cpp" "$base_name.bck"
    echo "No changes to $base_name.cpp..."
  else
    echo "Making $base_name.o..."
    g++ -g -c "$base_name.o" -o "$base_name.cpp" "$2"
    cp "$base_name.cpp" "$base_name.bck"
  fi
}

cd /home/ubuntu/2pc2drc/pisound-gamepad
echo "Cleaning..."
rm -rf src/*.o src/*/*.o

echo "Compiling..."
for file in src/*.c src/*/*.c; do
  if [ ! -e "$file" ]; then continue; fi
  make_c "$file"
done

for file in src/*.cpp; do
  if [ ! -e "$file" ]; then continue; fi
  if [ "$file" == "src/GamepadInput.cpp" ]; then
    make_cpp "$file" -l drc
  elif [ "$file" == "src/main.cpp" ]; then
    make_cpp "$file" -l drc -lpthread -lx264 -lm -ldl -lswscale
  else
    make_cpp "$file"
  fi
done

for file in src/sprite_types/*.cpp; do
  if [ ! -e "$file" ]; then continue; fi
  make_cpp "$file" -l drc
done

echo "Linking..."
g++ -g src/*.o src/*.h src/*/*.o src/*/*.h -o ../executable -l drc -lpthread -lx264 -lm -ldl -lswscale

#echo "Making okayu.o..."
#gcc -c -o src/images/okayu.o src/images/okayu.c
#echo "Making module.o..."
#gcc -c -o src/images/module.o src/images/module.c
#echo "Making jack.o..."
#gcc -c -o src/images/jack.o src/images/jack.c
#echo "Making GamepadScreen.o..."
#g++ -g -c src/GamepadScreen.cpp -o src/GamepadScreen.o
#echo "Making GamepadInput.o..."
#g++ -g -c src/GamepadInput.cpp -o src/GamepadInput.o -l drc
#echo "Making Character.o..."
#g++ -g -c src/sprite_types/Character.cpp -o src/sprite_types/Character.o -l drc
#echo "Making Cable.o..."
#g++ -g -c src/sprite_types/Cable.cpp -o src/sprite_types/Cable.o -l drc
#echo "Making Wire.o..."
#g++ -g -c src/sprite_types/Wire.cpp -o src/sprite_types/Wire.o -l drc
#echo "Making main.o..."
#g++ -g -c src/main.cpp -o src/main.o -l drc -lpthread -lx264 -lm -ldl -lswscale
#echo "Linking..."
#g++ -g src/*.o src/*.h src/*/*.o src/*/*.h -o ../executable -l drc -lpthread -lx264 -lm -ldl -lswscale
#echo "Done!"
