add_library(rgf
    glad/gl.h
    glad/gl.c
    app.hpp
    app.cpp
    ../../include/rgf.hpp
    renderer.hpp
    renderer.cpp
    shaders.hpp
    shaders.cpp
    textures.hpp
    textures.cpp
)

find_package(SDL2 REQUIRED)

target_compile_features(rgf PRIVATE cxx_std_17)

target_include_directories(rgf
    PUBLIC
        ${SDL2_INCLUDE_DIR}    
        ../../include
)

target_link_directories(rgf
    PUBLIC
        ${SDL2_LIBRARY}
)

target_link_libraries(rgf
    SDL2
    opengl32
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET rgf PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()