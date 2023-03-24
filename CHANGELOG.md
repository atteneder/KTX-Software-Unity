# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Emscripten 3.1.8 build (for Unity 2022.2 and newer)

## [1.0.0] - 2022-11-28
### Added
- `ktx_get_image_offset` (wraps `ktxTexture_GetImageOffset`)
- `ktx_get_image_size` (wraps `ktxTexture_GetImageSize`)
- `ktx_get_baseDepth`
- Experimental Unity Plugin implementation that enables uploading textures directly via native Graphics API
  - OpenGL (working with inaccurate sampling; enable via `KTX_UNITY_FEATURE_OPENGL` CMake option)
  - Vulkan (broken; WIP; enable via `KTX_UNITY_FEATURE_VULKAN` CMake option)
- (CI): iOS Simulator binaries
### Changed
- KTX-Software was updated to recent dev version (post 4.1.0-rc3)
- Added parameters `startLevel`,`layer` and `faceSlice` to `ktx_copy_data_levels_reverted`, so you can select a specific layer/face/slice and its level(s) to copy.

## [0.4.3] - 2022-04-12
### Fixed
- macOS binary type is `MinSizeRel` now (was default, which is `Debug` inadvertently)
- (CI) Fixed OS versions to avoid upgrading errors

## [0.4.2] - 2021-07-16
### Fixed
- Enabled embed bitcode on iOS libraries where it was still missing

## [0.4.1] - 2021-07-02
### Changed
- Android NDK updated to r21e
- CI: Installing Android NDK via GH action
### Fixed
- Added missing library (iOS libobj_basisu_cbind.a) to artifact
- Certified that KTX-Software BasisU C binding symbols are preserved (for Android)
- Switched to data-model-independent types in C binding to avoid crashes on certain platforms (e.g. iOS-arm64)

## [0.4.0] - 2021-06-30
### Added
- Support for Apple Silicon on macOS via a universal library
### Changed
- Updated KTX-Software (post 4.0.0 release; commit 15369663a43d72972dfe0a7e3597d3ef6d90b6b9)
- Enabled embed bitcode on iOS libraries
### Fixed
- Properly exposing functions on Windows
- Obj_basisu_cbind is now a static lib (no object lib anymore). Linking accordingly
- Preserve BasisU C binding on Windows

## [0.3.0] - 2021-01-31
### Added
- Universal Windows Platform architectures
  - ARM
  - ARM64 (Microsoft Hololens2)
  - x86
  - x64
### Changed
- Updated to KTX-Software branch based on 4.0.0 beta 6

## [0.2.4] - 2020-11-11
### Added
- Automatically add artifacts to release in GitHub action

## [0.2.3] - 2020-11-11
### Fixed
- `ktxTexture2_TranscodeBasis` was missing on Linux (at least). Added explicit reference to force linking.

## [0.2.2] - 2020-11-04
### Fixed
- ktxTexture2* functions are exported properly for Windows now

## [0.2.1] - 2020-11-04
### Changed
- Update to KTX-Software custom branch with Android support and basisu_c_binding fixes.
### Fixed
- Loading basis files now works on Windows x64

## [0.2.0] - 2020-10-28
### Changed
- Updated KTX-Software to 4.0.0 beta 5
- `ktx_load_ktx` does not check for KTX class anymore. It's up to users to detect unsupported KTX versions.
- Removed obsolete and incorrect `ktx_get_has_alpha`
- Removed redundant `ktx_get_num_components`
- Removed redundant `ktx_transcode_ktx`

## [0.1.0] - 2020-10-23
### Added
- Initial Version based on a custom fork of KTX-Software 4.0.0 beta 4 (incl. Android support and improved C binding)
