add_executable(pixeffect
    main.cpp
)

target_compile_features(pixeffect PRIVATE cxx_std_17)

target_link_libraries(pixeffect PRIVATE rgf)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET pixeffect PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    target_link_options(pixeffect PRIVATE -SUBSYSTEM:WINDOWS -ENTRY:mainCRTStartup)
endif()