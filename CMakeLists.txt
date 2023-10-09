cmake_minimum_required(VERSION 3.23)
project(IsometricEngine C)

set(CMAKE_C_STANDARD 99)

add_executable(IsometricEngine main.c includes/Graphes.h includes/struct.h src/graphes.c src/display.c src/algoChemin.c)

target_link_libraries(IsometricEngine -lmingw32 -lalleg44)
