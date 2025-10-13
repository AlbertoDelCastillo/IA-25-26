# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Sistemas Operativos
#
# @author Alberto Del Castillo Díaz
# @date Nov 25, 2024
# @brief CMakeLists.txt. You can use this as a template for your projects
# CMakeLists.txt defines the whole build
# CMake reads CMakeLists.txt sequentially
#
# Build process:
#   1. cd <project_folder>
#   2. mkdir build
#   3. cd build
#   4. cmake ..
#   5. make

cmake_minimum_required(VERSION 3.1)  # Mandatory

# set the project name 
project(BusquedasNoInformadas                # Mandatory
    VERSION 1.0
    DESCRIPTION "A simple CMake project for the BusquedasNoInformadas application"
    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17) 						# Use c++17
set(CMAKE_CXX_STANDARD_REQUIRED True)

# tell cmake to output binaries here:
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

# Debug and release flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -Weffc++")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# More Debug and release flags for c++23
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -Wall -Wextra -Werror -Wpedantic \
# -Wshadow -Wnon-virtual-dtor -Wold-style-cast \
# -Wcast-align -Wunused -Woverloaded-virtual \
# -Wconversion -Wsign-conversion -Wnull-dereference \
# -Wdouble-promotion -Wformat=2 -Wmisleading-indentation \
# -Wduplicated-cond -Wduplicated-branches -Wlogical-op \
# -Wuseless-cast -fsanitize=address,undefined,leak")

# tell cmake where to look for *.h files
include_directories(${PROJECT_SOURCE_DIR}/src)

# Adds an executable target called "BusquedasNoInformadas" to be built from the source files listed in the command invocation.
add_executable(BusquedasNoInformadas
    src/grafo.h
    src/grafo.cc
    src/funciones_cliente.h
    src/funciones_cliente.cc
    src/busquedas_no_informadas_main.cc)

# Add expected compiler features to a target.
target_compile_features(BusquedasNoInformadas PRIVATE cxx_std_23)

# Add compile options to a target.
target_compile_options(BusquedasNoInformadas PRIVATE -Wall -Wextra -Wpedantic -Weffc++)