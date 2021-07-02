// Copyright (c) 2020 Andreas Atteneder, All Rights Reserved.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//    http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#if defined(_WIN32)
  #if !defined(KTX_UNITY_API)
    #define KTX_UNITY_API __declspec(dllimport)
  #endif
#elif defined(__ANDROID__)
  #define KTX_UNITY_API __attribute__((visibility("default")))
#else
  #define KTX_UNITY_API
#endif


#include <stdint.h>
#include <string.h>

#include <ktx.h>

#include <basisu_c_binding.h>

#ifdef __cplusplus
extern "C" {
#endif

KTX_UNITY_API ktxTexture* ktx_load_ktx( const uint8_t * data, uint32_t length, KTX_error_code* out_status ) {
    
    KTX_error_code result;
    
    ktxTexture* newTex = 0;
    
    result = ktxTexture_CreateFromMemory(
        (const ktx_uint8_t*) data,
        (ktx_size_t) length,
        KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
        &newTex
        );

    *out_status = result;
    return newTex;
}

KTX_UNITY_API class_id ktx_get_classId ( ktxTexture* ktx ) {
    return ktx->classId;
}
KTX_UNITY_API ktx_bool_t ktx_get_isArray ( ktxTexture* ktx ) {
    return ktx->isArray;
}
KTX_UNITY_API ktx_bool_t ktx_get_isCubemap ( ktxTexture* ktx ) {
    return ktx->isCubemap;
}
KTX_UNITY_API ktx_bool_t ktx_get_isCompressed ( ktxTexture* ktx ) {
    return ktx->isCompressed;
}
KTX_UNITY_API ktx_uint32_t ktx_get_baseWidth ( ktxTexture* ktx ) {
    return ktx->baseWidth;
}
KTX_UNITY_API ktx_uint32_t ktx_get_baseHeight ( ktxTexture* ktx ) {
    return ktx->baseHeight;
}
KTX_UNITY_API ktx_uint32_t ktx_get_numDimensions ( ktxTexture* ktx ) {
    return ktx->numDimensions;
}
KTX_UNITY_API ktx_uint32_t ktx_get_numLevels ( ktxTexture* ktx ) {
    return ktx->numLevels;
}
KTX_UNITY_API ktx_uint32_t ktx_get_numLayers ( ktxTexture* ktx ) {
    return ktx->numLayers;
}
KTX_UNITY_API ktx_uint32_t ktx_get_numFaces ( ktxTexture* ktx ) {
    return ktx->numFaces;
}
KTX_UNITY_API ktx_uint32_t ktx_get_vkFormat ( ktxTexture* ktx ) {
    if (ktx->classId != ktxTexture2_c) {
        return 0; // VK_FORMAT_UNDEFINED
    }
    return ((ktxTexture2*)ktx)->vkFormat;
}
KTX_UNITY_API ktxSupercmpScheme ktx_get_supercompressionScheme ( ktxTexture* ktx ) {
    if (ktx->classId != ktxTexture2_c) {
        return KTX_SS_NONE;
    }
    return ((ktxTexture2 *)ktx)->supercompressionScheme;
}
KTX_UNITY_API ktx_uint32_t ktx_get_orientation ( ktxTexture* ktx ) {
    ktx_uint32_t orientation = 0;
    if(ktx->orientation.x == KTX_ORIENT_X_LEFT) {
        orientation |= 0x1;
    }
    if(ktx->orientation.y == KTX_ORIENT_Y_UP) {
        orientation |= 0x2;
    }
    if(ktx->orientation.z == KTX_ORIENT_Z_IN) {
        orientation |= 0x4;
    }
    return orientation;
}

ktx_uint32_t ktx_transcode (ktxTexture2* ktx, ktx_transcode_fmt_e fmt, ktx_transcode_flags transcodeFlags) {
    return ktxTexture2_TranscodeBasis(ktx, fmt, transcodeFlags);
}

KTX_UNITY_API void ktx_get_data(
    ktxTexture* ktx,
    const uint8_t ** data,
    uint32_t* length
    )
{
    *data = ktx->pData;
    *length = (uint32_t) ktx->dataSize;
}

/**
 * @~English
 * @brief Copies the texture's data into a destination buffer in reverted
 *        level order (resulting in biggest to smallest)
 *
 * @param[in] ktx pointer to the ktxTexture object of interest.
 * @param[in] ktx pointer to the destination buffer.
 * @param[in] dst_length length of the destination buffer.
 * 
 * @return    KTX_SUCCESS on success, other KTX_* enum values on error.
 */
KTX_UNITY_API KTX_error_code ktx_copy_data_levels_reverted(
    ktxTexture* ktx,
    uint8_t * dst,
    uint32_t dst_length
    )
{
    size_t dst_offset = 0;
    KTX_error_code result;

    if(ktx->dataSize>dst_length) return KTX_FILE_OVERFLOW;

    for (ktx_uint32_t level = 0; level < ktx->numLevels; level++)
    {
        size_t offset;
        result = ktxTexture_GetImageOffset(
            ktx,
            level,
            0, //layer
            0, //faceSlice
            &offset
            );
        if(result!=KTX_SUCCESS) return result;
        ktx_size_t level_size = ktxTexture_GetImageSize(ktxTexture(ktx),level);
        if(dst_offset+level_size > dst_length) {
            return KTX_FILE_OVERFLOW;
        }
        memcpy((void*)(dst+dst_offset), ktx->pData+offset, level_size);
        dst_offset += level_size;
    }
    return result;
}

KTX_UNITY_API void ktx_unload_ktx( ktxTexture* ktx ) {
    ktxTexture_Destroy(ktx);
}

void _basisu_dummy() {
    // Referencing just one function of KTX-Software's basisu C binding
    // ensures its symbols are included when linking (e.g. Android)
    basis_file* x = ktx_basisu_create_basis();
}

#ifdef __cplusplus
}
#endif
