cmake_minimum_required(VERSION 3.15)

project(
    KTX-Software-Unity
    VERSION 1.0.0
    DESCRIPTION "Unity wrapper for KTX-Software"
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(KTX_FEATURE_FORCE_STATIC ON)
add_subdirectory(KTX-Software)

set(UNITY_TYPE MODULE)
if(IOS OR EMSCRIPTEN)
    set(UNITY_TYPE STATIC)
endif()

add_library(ktx_unity ${UNITY_TYPE}
    src/ktx_c_binding.c
)

target_link_libraries(ktx_unity ktx_read)

if(APPLE)
    set_target_properties(ktx_unity PROPERTIES BUNDLE true)
endif()
