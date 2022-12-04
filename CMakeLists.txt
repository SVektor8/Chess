cmake_minimum_required(VERSION 3.23)
project(Chess)

set(CMAKE_CXX_STANDARD 23)

add_executable(Chess main.cpp)

set(SFML_DIR "C:\\Users\\Viktor\\Documents\\Chess\\lib\\sfml\\SFML-2.5.1\\lib\\cmake\\SFML")

target_link_libraries(Chess sfml-graphics)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2.5 REQUIRED graphics network audio window system)
