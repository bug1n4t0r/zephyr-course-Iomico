cmake_minimum_required(VERSION 3.20.0)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PROJECT_DIR)

list(APPEND BOARD_ROOT "${PROJECT_DIR}")

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE src/main.cpp src/sensor_shell.c)
add_subdirectory("../driver_module/drivers/bugis_driver" 
                 "${CMAKE_CURRENT_BINARY_DIR}/bugis_driver")
