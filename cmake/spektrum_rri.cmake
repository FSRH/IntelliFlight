
# includes spektrum_rri driver
find_file(SPEKTRUM_RRI_DIR "lib/spektrum_rri" "${CMAKE_CURRENT_SOURCE_DIR}")
if (SPEKTRUM_RRI_DIR STREQUAL "SPEKTRUM_RRI_DIR-NOTFOUND")
    message(FATAL_ERROR "Could not locate lib/spektrum_rri directory")
endif ()

include_directories(${SPEKTRUM_RRI_DIR})

file(GLOB_RECURSE SPEKTRUM_RRI_FILES ${SPEKTRUM_RRI_DIR}/*.h ${SPEKTRUM_RRI_DIR}/*.c)

add_library(spektrum_rri STATIC ${SPEKTRUM_RRI_FILES})
