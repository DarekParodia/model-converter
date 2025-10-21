cmake_minimum_required(VERSION 3.28)
project(vlknd VERSION 0.1.0)

set(PROJECT_DISPLAYNAME "Model Converter")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set include directories
set(SrcDIR ${CMAKE_SOURCE_DIR}/src)
set(IncludeDIR ${CMAKE_SOURCE_DIR}/include)
set(CMakeDIR ${CMAKE_SOURCE_DIR}/cmake)

include_directories(
        ${SrcDIR}
        ${IncludeDIR}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# cxxopts
add_subdirectory(${IncludeDIR}/cxxopts)
include_directories(${cxxopts_INCLUDE_DIRS})

# spdlog
add_subdirectory(${IncludeDIR}/spdlog)
include_directories(${spdlog_INCLUDE_DIRS})

# json
add_subdirectory(${IncludeDIR}/nlohmann_json)
include_directories(${nlohmann_json_INCLUDE_DIRS})

add_executable(converter src/main.cpp src/encoder.cpp)

target_link_libraries(converter PRIVATE cxxopts)
target_link_libraries(converter PRIVATE spdlog)
target_link_libraries(converter PRIVATE nlohmann_json)