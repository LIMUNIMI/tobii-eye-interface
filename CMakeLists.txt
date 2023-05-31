cmake_minimum_required(VERSION 3.14)

set (TargetName "eye-interface") # imposta nome progetto
project(${TargetName})

set(CMAKE_CXX_STANDARD 14)
add_executable(${TargetName} main.cpp)
target_link_libraries(eye-interface /usr/lib/tobii/libtobii_stream_engine.so) # su ubuntu
target_link_libraries(eye-interface libevdev.a)
#target_link_libraries(${TargetName} /run/media/fabio/074adaa2-deb6-4e42-936b-dac103bd9e2f/usr/lib/tobii/libtobii_stream_engine.so) #su fedora

target_link_libraries(${TargetName} pthread)

