set(app_sources
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/lib/graphics/src/Texture.cpp
    ${CMAKE_SOURCE_DIR}/lib/graphics/src/VGADisplayController.cpp
    ${CMAKE_SOURCE_DIR}/lib/graphics/src/VGADisplayController3Bit.cpp
    ${CMAKE_SOURCE_DIR}/lib/game/src/Environment3D.cpp
)

idf_component_register(SRCS ${app_sources})