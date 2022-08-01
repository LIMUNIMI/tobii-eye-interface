cmake_minimum_required(VERSION 3.13)
project(example)

set(CMAKE_CXX_STANDARD 14)
add_executable(example main.cpp)
#target_link_libraries(example /usr/lib/tobii/libtobii_stream_engine.so)
target_link_libraries(example /run/media/fabio/074adaa2-deb6-4e42-936b-dac103bd9e2f/usr/lib/tobii/libtobii_stream_engine.so)

target_link_libraries(example pthread)

