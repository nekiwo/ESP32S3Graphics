cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# LittleFS for ESP-IDF
get_filename_component(configName "${CMAKE_BINARY_DIR}" NAME)
list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/.pio/libdeps/${configName}/esp_littlefs")

# Create filesystem image
# message(test1: ${configName})
# littlefs_create_partition_image(littlefs ${CMAKE_SOURCE_DIR}/data FLASH_IN_PROJECT)

project(texturemapping)