cmake_minimum_required(VERSION 3.16)
project(MiniShell LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Recursively find all source and header files from all subdirectories
file(GLOB_RECURSE SRCS
    "include/*.cpp" "include/*.cpp" "src/*.cpp" "src/*.cpp" 
)

# Create a single executable from all source files
add_executable(${PROJECT_NAME} ${SRCS})