# KTX Library for Unity

> The [KTX file format](http://github.khronos.org/KTX-Specification/), version 2 is a format for storing textures for GPU applications.

This projects provides a C wrapper for [KTX-Software](https://github.com/KhronosGroup/KTX-Software) including C wrappers for Basis Universal (.basis format) and a CMake configuration to build native libraries for following platforms:

- macOS x86_64 (Intel)
- Windows 64-bit
- Linux x86_64
- iOS (arm64 and armv7a)
- Android (arm64-v8a and armeabi-v7a)

Those native libraries are used by [KtxUnity](https://github.com/atteneder/KtxUnity), which provides access to KTX files within [Unity](https://unity3d.com)

## License

Copyright (c) 2020 Andreas Atteneder, All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use files in this repository except in compliance with the License.
You may obtain a copy of the License at

   <http://www.apache.org/licenses/LICENSE-2.0>

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
