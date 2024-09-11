cmake_minimum_required(VERSION 3.16)
project(linux_server C)

set(CMAKE_C_STANDARD 90)
#set(GCC_COVERAGE_COMPILE_FLAGS "-g3 -Wall -Wextra -Wpedantic -O0 -std=c89")
set(GCC_COVERAGE_COMPILE_FLAGS "-g3 -Wall -Wextra -Wpedantic -O0 -std=c89 -m32 -Werror")
#set(GCC_COVERAGE_COMPILE_FLAGS_2 "-fshort-enums") #8bit enums
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS_2}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin) # add_executable() .exe & add_library(SHARED) .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib) # ??? maybe linux add_library(SHARED) .so goes there
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib) # add_library(STATIC) .a & add_library(SHARED) .dll.a

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_executable(${PROJECT_NAME}
        ./src/main.c
        src/server_udp.c
        ./src/irrigation_data.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/straykovsky_standard_library/include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/straykovsky_standard_library/lib/straykovsky_standard_library.so)
