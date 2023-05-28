add_executable(fps
    main.cpp
)

target_compile_features(fps PRIVATE cxx_std_17)

target_link_libraries(fps PRIVATE rgf)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET fps PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    target_link_options(fps PRIVATE -SUBSYSTEM:WINDOWS -ENTRY:mainCRTStartup)
endif()