cmake_minimum_required(VERSION 3.10)

# set(CMAKE_LANGUAGE_COMPILER /usr/bin/g++ CACHE STRING "Set to g++" FORCE)
project("Cycling_Fit_Analyzer" LANGUAGES CXX)

add_executable(beta src/main.cpp src/my_listener.cpp)
target_include_directories(beta PUBLIC include /lib/fitsdk)

file(GLOB fitsdk_SRC "lib/fitsdk/*.hpp" "lib/fitsdk/*.cpp")
add_library(fitsdk STATIC "${fitsdk_SRC}")

target_link_libraries(beta PRIVATE fitsdk)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)