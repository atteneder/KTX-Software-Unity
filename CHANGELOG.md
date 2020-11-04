# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
