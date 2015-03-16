cmake_minimum_required(VERSION 2.8)

project(cupido CXX)

include_directories(
    "../Clarus/include"
    "../Cight/include"
    "include"
)

find_package(Boost REQUIRED COMPONENTS filesystem system thread)
find_package(OpenCV 2.4.8 REQUIRED)

add_library(clarus_core STATIC IMPORTED)
add_library(clarus_io STATIC IMPORTED)
add_library(clarus_model STATIC IMPORTED)
add_library(clarus_vision STATIC IMPORTED)
add_library(cight STATIC IMPORTED)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set_target_properties(clarus_core PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/debug/libclarus_core.a"
    )

    set_target_properties(clarus_io PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/debug/libclarus_io.a"
    )

    set_target_properties(clarus_model PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/debug/libclarus_model.a"
    )

    set_target_properties(clarus_vision PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/debug/libclarus_vision.a"
    )

    set_target_properties(cight PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Cight/build/debug/libcight.a"
    )
else()
    set_target_properties(clarus_core PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/release/libclarus_core.a"
    )

    set_target_properties(clarus_io PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/release/libclarus_io.a"
    )

    set_target_properties(clarus_model PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/release/libclarus_model.a"
    )

    set_target_properties(clarus_vision PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Clarus/build/release/libclarus_vision.a"
    )

    set_target_properties(cight PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/../Cight/build/release/libcight.a"
    )
endif()

add_executable(cupido
    "src/main.cpp"
)

target_link_libraries(cupido
    cight
    clarus_io
    clarus_vision
    clarus_model
    clarus_core
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
)
