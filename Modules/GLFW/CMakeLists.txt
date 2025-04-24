add_subdirectory(3rdparty/glfw)

add_library(Glfw SHARED
        Module.cpp
        DisplayGLFW.cpp
        NativeHelperGLFW.cpp
)
target_link_libraries(Glfw PRIVATE glfw HiveCore)
target_compile_definitions(Glfw PRIVATE HIVE_EXPORTS)
