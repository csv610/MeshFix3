# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.1.0] - 2026-05-20

### Added
- Comprehensive Doxygen configuration for API documentation
- `.clang-format` for consistent code formatting (LLVM style)
- Enhanced CI/CD pipeline with:
  - clang-format code style checks
  - clang-tidy static analysis
  - Code coverage reporting with gcovr
  - Doxygen documentation generation
- CMake options for BUILD_DOCS and ENABLE_COVERAGE
- Installation rules for proper Unix installation paths
- README badges for CI, coverage, and license
- CONTRIBUTING.md with development guidelines
- .editorconfig for cross-editor consistency
- .codecov.yml for coverage configuration

### Changed
- Expanded unit test suite from 3 to 45+ test functions
- Added tests for Point, List, Vertex, Edge, Triangle, Matrix, and Basic_TMesh classes
- Improved test assertions for more robust validation

### Removed
- Removed ~230 lines of commented-out legacy code from checkAndRepair.cpp

### Fixed
- Resolved compiler warnings in unit tests
- Fixed test assertions to work with actual API behavior

## [2.0.0] - 2024-01-01

### Added
- Modernized port with C++20 support
- Spaceship operator (`<=>`) for Point class comparison
- `std::string_view` for CLI argument parsing
- `std::span` for safer orientation predicate interfaces
- `-h` help flag and `-o` output file option
- Python integration tests (`test_cli.py`, `test_dataset.py`)
- C++ unit tests (`unit_tests.cpp`)
- CMake build system
- GitHub Actions CI/CD pipeline

### Removed
- Visual Studio project files (.vcxproj)
- Windows-specific compiler guards
- Legacy build scripts

## [1.0] - 2016-01-01

### Added
- Original MeshFix release by Marco Attene
- Mesh repair algorithms for digitized polygon meshes
- Support for OFF, PLY, STL, OBJ, VRML, IV formats
- Hole filling, degeneracy removal, and intersection repair
- Benchmark tools for performance analysis

---

## Version History

| Version | Date | Description |
|---------|------|-------------|
| 2.1.0 | 2026-05-20 | Software engineering improvements |
| 2.0.0 | 2024-01-01 | Modernized C++20 port |
| 1.0 | 2016-01-01 | Original MeshFix release |