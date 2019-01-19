# includes microcontroller interconnect network protocol
find_file(MIN_PROTO_DIR "lib/min_proto" "${CMAKE_CURRENT_SOURCE_DIR}")
if (MIN_PROTO_DIR STREQUAL "MIN_PROTO_DIR-NOTFOUND")
    message(FATAL_ERROR "Could not locate lib/min_proto directory")
endif ()

include_directories(${MIN_PROTO_DIR})

file(GLOB_RECURSE MIN_PROTO_FILES ${MIN_PROTO_DIR}/target/min.h ${MIN_PROTO_DIR}/target/min.c)

add_library(min_proto STATIC ${MIN_PROTO_FILES})

