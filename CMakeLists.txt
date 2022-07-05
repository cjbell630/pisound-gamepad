cmake_minimum_required(VERSION 3.16.3)
project(pisound_gamepad)

set(CMAKE_CXX_STANDARD 14)


add_executable(pisound_gamepad
        src/main.cpp src/GamepadScreen.cpp src/Sprite.cpp src/Sprite.h src/GamepadInput.cpp src/GamepadInput.h)
target_link_libraries(pisound_gamepad drc pthread x264 m dl swscale)