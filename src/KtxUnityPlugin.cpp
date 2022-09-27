// Copyright (c) 2020-2022 Andreas Atteneder, All Rights Reserved.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//    http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "IUnityInterface.h"
#include "IUnityGraphics.h"
#include "ThreadSafeQueue.h"

#include <ktx.h>

#include <math.h>
#include <unordered_map>

#ifdef KTX_UNITY_FEATURE_OPENGL
#include "GL/glcorearb.h"
#endif

typedef struct ktxTextureResult {
    void* texture = 0;
    GLenum target;
    GLenum error;
} ktxTextureResult;

static ThreadSafeQueue<ktxTexture*> g_ktxTextureQueue;
static std::unordered_map<ktxTexture*,ktxTextureResult> g_ktxTextureResults;

extern "C" void
UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
ktx_enqueue_upload(ktxTexture* ktx) {
    g_ktxTextureQueue.push(ktx);
}

extern "C" bool
UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
ktx_dequeue_upload(ktxTexture* ktx, void** texture, int32_t* error) {
    const auto result = g_ktxTextureResults.find(ktx);
    if(result!=g_ktxTextureResults.end()) {
        *texture = result->second.texture;
        *error = result->second.error;
        g_ktxTextureResults.erase(result);
        return true;
    }
    return false;
}

static void UNITY_INTERFACE_API
OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);

static IUnityInterfaces *s_UnityInterfaces = NULL;
static IUnityGraphics *s_Graphics = NULL;

extern "C" void
UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginLoad(IUnityInterfaces *unityInterfaces) {
    s_UnityInterfaces = unityInterfaces;
    s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
    s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" void UNITY_INTERFACE_EXPORT

UNITY_INTERFACE_API UnityPluginUnload() {
    s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;

static void UNITY_INTERFACE_API
OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType) {

    if (eventType == kUnityGfxDeviceEventInitialize) {
        s_DeviceType = s_Graphics->GetRenderer();
    }

    if (eventType == kUnityGfxDeviceEventShutdown) {
        s_DeviceType = kUnityGfxRendererNull;
    }
}

static void UNITY_INTERFACE_API

OnRenderEvent(int eventID) {
    if (eventID == 1) {

        auto ktx = g_ktxTextureQueue.pop();
        ktxTextureResult result;

        if( s_DeviceType == kUnityGfxRendererOpenGLCore
            || s_DeviceType == kUnityGfxRendererOpenGLES20
            || s_DeviceType == kUnityGfxRendererOpenGLES30
            )
        {
            while(ktx) {
                ktxTexture_GLUpload(
                        ktx.value(),
                        (GLuint*)&result.texture,
                        &result.target,
                        &result.error);

                g_ktxTextureResults[ktx.value()] = result;
                ktx = g_ktxTextureQueue.pop();
            }
        }
    }
}

// --------------------------------------------------------------------------
// GetRenderEventFunc, an example function we export which is used to get a rendering event callback function.

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT

UNITY_INTERFACE_API GetRenderEventFunc() {
    return OnRenderEvent;
}
