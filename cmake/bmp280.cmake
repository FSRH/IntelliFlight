# includes BMP280 driver
find_file(BMP280_DRIVER_DIR "lib/BMP280_driver" "${CMAKE_CURRENT_SOURCE_DIR}")
if (BMP280_DRIVER_DIR STREQUAL "BMP280_DRIVER_DIR-NOTFOUND")
    message(FATAL_ERROR "Could not locate lib/BMP280_driver directory")
endif ()

include_directories(${BMP280_DRIVER_DIR})

file(GLOB_RECURSE BMP280_DRIVER_FILES ${BMP280_DRIVER_DIR}/*.h ${BMP280_DRIVER_DIR}/*.c)

add_library(bmp280 STATIC ${BMP280_DRIVER_FILES})

