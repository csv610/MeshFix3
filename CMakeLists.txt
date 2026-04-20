cmake_minimum_required(VERSION 3.20)
project(MeshFix VERSION 2.1 LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_SOURCES
    "${PROJECT_SOURCE_DIR}/src/TMesh/edge.cpp"
    "${PROJECT_SOURCE_DIR}/src/TMesh/vertex.cpp"
    "${PROJECT_SOURCE_DIR}/src/TMesh/io.cpp"
    "${PROJECT_SOURCE_DIR}/src/TMesh/triangle.cpp"
    "${PROJECT_SOURCE_DIR}/src/TMesh/tin.cpp"
    "${PROJECT_SOURCE_DIR}/src/Algorithms/detectIntersections.cpp"
    "${PROJECT_SOURCE_DIR}/src/Algorithms/holeFilling.cpp"
    "${PROJECT_SOURCE_DIR}/src/Algorithms/marchIntersections.cpp"
    "${PROJECT_SOURCE_DIR}/src/Algorithms/checkAndRepair.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/heap.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/matrix.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/orientation.c"
    "${PROJECT_SOURCE_DIR}/src/Kernel/list.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/coordinates.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/tmesh.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/graph.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/point.cpp"
    "${PROJECT_SOURCE_DIR}/src/Kernel/jqsort.cpp"
)

add_executable(MeshFix ${COMMON_SOURCES} "${PROJECT_SOURCE_DIR}/src/MeshFix/meshfix.cpp")
add_executable(unit_tests ${COMMON_SOURCES} "${PROJECT_SOURCE_DIR}/tests/unit_tests.cpp")

target_include_directories(MeshFix PUBLIC
    "${PROJECT_SOURCE_DIR}/include/TMesh"
    "${PROJECT_SOURCE_DIR}/include/Kernel"
    "${PROJECT_SOURCE_DIR}/src/TMesh"
    "${PROJECT_SOURCE_DIR}/src/Kernel"
    "${PROJECT_SOURCE_DIR}/src/Algorithms"
)

target_include_directories(unit_tests PUBLIC
    "${PROJECT_SOURCE_DIR}/include/TMesh"
    "${PROJECT_SOURCE_DIR}/include/Kernel"
    "${PROJECT_SOURCE_DIR}/src/TMesh"
    "${PROJECT_SOURCE_DIR}/src/Kernel"
    "${PROJECT_SOURCE_DIR}/src/Algorithms"
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64-bit compiler detected")
    target_compile_definitions(MeshFix PUBLIC IS64BITPLATFORM)
    target_compile_definitions(unit_tests PUBLIC IS64BITPLATFORM)
endif()
set(BIN_FOLDER "bin")

set_target_properties(MeshFix PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/${BIN_FOLDER}"
)

set_target_properties(unit_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/${BIN_FOLDER}"
)

target_compile_options(MeshFix PRIVATE -Wall -Wextra -Wno-unused-parameter)
target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wno-unused-parameter)
