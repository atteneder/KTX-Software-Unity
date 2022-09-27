cmake_minimum_required(VERSION 3.15)

project(
    KTX-Software-Unity
    VERSION 0.2.2
    DESCRIPTION "Unity wrapper for KTX-Software"
)

include(CMakeDependentOption)

CMAKE_DEPENDENT_OPTION( KTX_UNITY_FEATURE_VULKAN
    "Enable Vulkan texture upload"
    ON
    "NOT IOS;NOT EMSCRIPTEN"
    OFF
    )
option( KTX_UNITY_FEATURE_OPENGL "Enable OpenGL texture upload" ON )

if(APPLE)
    # Signing
    set(XCODE_CODE_SIGN_IDENTITY "Development" CACHE STRING "Xcode code sign ID")
    set(XCODE_DEVELOPMENT_TEAM "" CACHE STRING "Xcode development team ID")
    # Deployment
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "macOS Deployment Target")
    if(IOS)
        set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "11.0" CACHE STRING "iOS Deployment Target")
    endif()
endif()

if(KTX_UNITY_FEATURE_VULKAN OR KTX_UNITY_FEATURE_OPENGL)
    set(KTX_UNITY_FEATURE_GPU_UPLOAD TRUE)
endif()

if(KTX_UNITY_FEATURE_VULKAN)
    find_package(Vulkan REQUIRED)
    set(KTX_FEATURE_VULKAN TRUE)
endif()

if(KTX_UNITY_FEATURE_OPENGL)
    find_package(OpenGL)
    set(KTX_FEATURE_GL_UPLOAD TRUE)

    find_path(GLES2_INCLUDE_DIR GLES2/gl2.h
            HINTS ${ANDROID_NDK})
    find_library(GLES2_LIBRARY libGLESv2.so
            HINTS ${GLES2_INCLUDE_DIR}/../lib)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(KTX_FEATURE_STATIC_LIBRARY ON)
add_subdirectory(KTX-Software)

set(UNITY_TYPE MODULE)
if(IOS OR EMSCRIPTEN)
    set(UNITY_TYPE STATIC)
endif()

add_library(ktx_unity ${UNITY_TYPE}
    src/ktx_c_binding.cpp

    )

if(KTX_UNITY_FEATURE_GPU_UPLOAD)
    target_sources(ktx_unity PRIVATE
            src/KtxUnityPlugin.cpp
            )
    target_compile_features(ktx_unity PRIVATE cxx_std_17)
endif()


target_include_directories(ktx_unity PRIVATE
        Unity
        KTX-Software/other_include
)

target_link_libraries(ktx_unity
    ktx_read
    obj_basisu_cbind
)

if(KTX_UNITY_FEATURE_GPU_UPLOAD)
    target_compile_definitions(ktx_unity PRIVATE KTX_UNITY_FEATURE_GPU_UPLOAD)
endif()

if(KTX_UNITY_FEATURE_VULKAN)
    target_compile_definitions(ktx_unity PRIVATE KTX_UNITY_FEATURE_VULKAN)
    target_include_directories(ktx_unity PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(ktx_unity Vulkan::Vulkan)
endif()

if(KTX_UNITY_FEATURE_OPENGL)
    target_compile_definitions(ktx_unity PRIVATE KTX_UNITY_FEATURE_OPENGL)
    if(ANDROID)
        target_include_directories(ktx_unity PRIVATE ${GLES2_INCLUDE_DIR})
        target_compile_definitions(ktx_unity PRIVATE KTX_UNITY_FEATURE_OPENGL_ES)
    elseif(IOS)
        target_compile_definitions(ktx_unity PRIVATE KTX_UNITY_FEATURE_OPENGL_ES)
    endif()
endif()

if(APPLE)
    set_target_properties(ktx_unity PROPERTIES BUNDLE true)
    if(KTX_EMBED_BITCODE)
        target_compile_options(ktx_unity PRIVATE "-fembed-bitcode")
    endif()

    if(IOS)
        set_target_properties(ktx_read PROPERTIES
            FRAMEWORK FALSE
            )
    endif()
elseif(WIN32)
    target_sources(
        ktx_unity
    PRIVATE
        # Explicitely export some ktx_read functions
        src/ktx_unity_export.def
    )
    target_compile_definitions(
        ktx_unity
    PRIVATE 
        "KTX_UNITY_API=__declspec(dllexport)"
    )
endif()
