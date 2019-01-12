find_file(LOLASERIAL_DIR "lib/LoLaSerial" "${CMAKE_CURRENT_SOURCE_DIR}")
if (LOLASERIAL_DIR STREQUAL "LOLASERIAL_DIR-NOTFOUND")
    message(FATAL_ERROR "Could not locate lib/LoLaSerial directory")
endif ()

include_directories(${LOLASERIAL_DIR}/lib/)
include_directories(${LOLASERIAL_DIR}/include/)

file(GLOB_RECURSE LOLASERIAL_FILES ${LOLASERIAL_DIR}/**.h ${LOLASERIAL_DIR}/**.c ${LOLASERIAL_DIR}/**.hpp ${LOLASERIAL_DIR}/**.cpp)

add_library(lolaserial STATIC ${LOLASERIAL_FILES})

