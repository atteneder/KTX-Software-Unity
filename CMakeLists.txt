cmake_minimum_required(VERSION 3.15)

project(
    KTX-Software-Unity
    VERSION 0.2.2
    DESCRIPTION "Unity wrapper for KTX-Software"
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(KTX_FEATURE_STATIC_LIBRARY ON)
add_subdirectory(KTX-Software)

set(UNITY_TYPE MODULE)
if(IOS OR EMSCRIPTEN)
    set(UNITY_TYPE STATIC)
endif()

add_library(ktx_unity ${UNITY_TYPE}
    src/ktx_c_binding.cpp
    src/KtxUnityPlugin.cpp
    )

target_compile_features(ktx_unity PRIVATE cxx_std_17)

target_include_directories(ktx_unity PRIVATE
        Unity
)

target_link_libraries(ktx_unity
    ktx_read
    obj_basisu_cbind
)

if(APPLE)
    set_target_properties(ktx_unity PROPERTIES BUNDLE true)
    if(KTX_EMBED_BITCODE)
        target_compile_options(ktx_unity PRIVATE "-fembed-bitcode")
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
