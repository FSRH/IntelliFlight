cmake_minimum_required(VERSION 3.9)
project(IntelliFlight)

# Include an error in case the user forgets to specify ARM as a toolchain
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=cmake/arm-toolchain.cmake or use\
    your own toolchain file")
endif ()

if ((NOT ${CMAKE_C_COMPILER_ID} STREQUAL "GNU") OR
(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR
(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 7.2.1))
    message(WARNING "This project is targeted for ARM GCC versions 7.2.1 or above. Some features may not be supported\
     by your compiler. You are using:
     ${CMAKE_CXX_COMPILER_ID} C, version ${CMAKE_C_COMPILER_VERSION}
     ${CMAKE_CXX_COMPILER_ID} CXX, version ${CMAKE_CXX_COMPILER_VERSION}")
endif ()

# Set the C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Verbose logging
set(LOG_VERBOSE false)

# Choose target device here
set(DEVICE STM32F765VGT6)

# Choose target mainboard pin configuration here
add_definitions(-DSTM32F765_DEV)

# Include libopencm3
include(cmake/libopencm3.cmake)

# Include doxygen for dank docs
include(cmake/doxygen.cmake)

# Include additional compilation flags
include(cmake/compile_flags.cmake)

# Include bmp280 api
include(cmake/bmp280.cmake)

# Collect sources into SOURCE_FILES variable and include your headers directory
file(GLOB_RECURSE SOURCE_FILES "src/*.c" "src/*.cpp")
file(GLOB_RECURSE INC_FILES "include/*.h" "include/*.tpp")
include_directories(src)

# Specify output executable
add_executable(${PROJECT_NAME}.elf main.cpp ${INC_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}.elf bmp280)

# Output elf file size
add_custom_command(
        TARGET ${PROJECT_NAME}.elf
        POST_BUILD
        COMMAND ${ARM_SIZE} ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

message("----------------------------------------")
