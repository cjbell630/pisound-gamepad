cmake_minimum_required(VERSION 3.16.3)
project(pisound_gamepad)

set(CMAKE_CXX_STANDARD 14)


add_executable(pisound_gamepad
        src/main.cpp src/GamepadScreen.cpp src/sprite_types/Sprite.h  src/GamepadInput.cpp src/GamepadInput.h src/sprite_types/Drawable.h src/images/okayu.h src/sprite_types/Character.cpp src/sprite_types/Character.h src/images/module.c src/sprite_types/Cable.cpp src/sprite_types/Cable.h src/sprite_types/Wire.cpp src/sprite_types/Wire.h)
target_link_libraries(pisound_gamepad drc pthread x264 m dl swscale)