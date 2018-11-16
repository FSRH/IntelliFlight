# Enforce C/C++ standard level
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Enforce to disable any compiler-specific extensions
set(CMAKE_C_EXTENSIONS NO)
set(CMAKE_CXX_EXTENSIONS NO)

# TODO: make asm() extension working
# Flags that apply to both C and C++
set(COMMON_WARN_FLAGS "${COMMON_WARN_FLAGS} -pedantic -Wall -Wextra \
-Wcast-align -Wcast-qual -Wconversion -Wdisabled-optimization -Wdouble-promotion -Wduplicated-cond \
-Wduplicated-branches -Wfloat-equal -Wformat=2 -Winit-self -Winline -Winvalid-pch -Wlogical-op \
-Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wno-unused -Wnull-dereference -Wodr \
-Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default \
-Wswitch-enum -Wwrite-strings -Wundef -Wuninitialized -Wunreachable-code")

# Flags that apply only to C OR C++
set(C_WARN_FLAGS "${COMMON_WARN_FLAGS} -Wbad-function-cast -Wmissing-prototypes -Wnested-externs \
-Wold-style-definition -Wstrict-prototypes")
set(CXX_WARN_FLAGS "${COMMON_WARN_FLAGS} -Wctor-dtor-privacy -Wnoexcept -Wold-style-cast -Woverloaded-virtual \
-Wsign-promo -Wstrict-null-sentinel -Wuseless-cast -Wzero-as-null-pointer-constant")

# Pass them back to the CMake variable
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARN_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARN_FLAGS}")

if (LOG_VERBOSE)
    message("-------------Warning Flags--------------")
    message(STATUS "C   : ${C_WARN_FLAGS}")
    message(STATUS "CXX : ${CXX_WARN_FLAGS}")
endif ()

# Get Build Information
execute_process(COMMAND make --version OUTPUT_VARIABLE MAKE_OUTPUT)
string(REGEX REPLACE "GNU Make ([0-9]\\.[0-9]\\.*[0-9]*).+" "\\1" MAKE_VERSION ${MAKE_OUTPUT})

message("-----------Build Information------------")
message(STATUS "Host    : ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "Make    : ${MAKE_VERSION}")
message(STATUS "CC      : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "CXX     : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build   : ${CMAKE_BUILD_TYPE}")

# Additional Flags
set(ADDITIONAL_C_FLAGS "-fmessage-length=0 -fno-strict-aliasing -ffunction-sections -fdata-sections -fsigned-char")
set(ADDITIONAL_CXX_FLAGS "-fno-exceptions -fno-rtti")
set(ADDITIONAL_LINKER_FLAGS "-Wl,-Map,${CMAKE_PROJECT_NAME}_${CMAKE_BUILD_TYPE}.map,--cref,--gc-sections")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra ${TARGET_FLAGS} ${ADDITIONAL_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ${TARGET_FLAGS} ${ADDITIONAL_CXX_FLAGS}")
set(LINKER_FLAGS "${LINKER_FLAGS} -nostartfiles -lc -lnosys --specs=rdimon.specs ${ADDITIONAL_LINKER_FLAGS}")

message("------------Additional Flags------------")
message(STATUS "C   : ${ADDITIONAL_C_FLAGS}")
message(STATUS "CXX : ${ADDITIONAL_CXX_FLAGS}")
message(STATUS "LD  : ${ADDITIONAL_LINKER_FLAGS}")

# Build-dependent flags
#set(CMAKE_C_FLAGS_DEBUG "-O0")
#set(CMAKE_CXX_FLAGS_DEBUG "-O0")
#set(CMAKE_C_FLAGS_RELEASE "-O2")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2")
#set(CMAKE_C_FLAGS_MINSIZEREL "-Os")
#set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
#set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Og")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og")

# Dump all the flags at this point
if (LOG_VERBOSE)
    string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

    message("-------------Full Flag List-------------")
    message(STATUS "C   : ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${BUILD_TYPE}}")
    message(STATUS "CXX : ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")
    message(STATUS "LD  : ${LINKER_FLAGS}")
endif()
