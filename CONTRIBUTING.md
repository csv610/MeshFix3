# Contributing to MeshFix3

Thank you for your interest in contributing to MeshFix3!

## Code of Conduct

This project adheres to a code of conduct that all contributors are expected to follow. Please be respectful and constructive in all interactions.

## How Can I Contribute?

### Reporting Bugs

Before submitting a bug report:
- Search the issue tracker to avoid duplicates
- Try to reproduce the issue with the latest version
- Include the mesh file that triggers the bug (if possible)

### Suggesting Enhancements

Feature requests are welcome! Please describe:
- The use case for the feature
- How it would benefit MeshFix3 users
- Any implementation ideas you have

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Style Guidelines

### C++ Code

- Follow the `.clang-format` configuration (LLVM style)
- Use C++20 features where appropriate
- Add Doxygen comments for public APIs
- Write unit tests for new functionality

### Commit Messages

- Use clear, descriptive commit messages
- Start with a verb (Add, Fix, Update, Remove)
- Reference issues when applicable

## Development Setup

```bash
# Clone your fork
git clone https://github.com/your-username/MeshFix3.git
cd MeshFix3

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . -j$(nproc)

# Run tests
./bin/unit_tests

# Generate documentation
make docs
```

## Testing

All new features should include tests:
- Unit tests for core functionality (`tests/unit_tests.cpp`)
- Integration tests for CLI behavior (`tests/test_cli.py`)
- Dataset tests for mesh repair operations (`tests/test_dataset.py`)

Run the full test suite:
```bash
python3 tests/test_cli.py
python3 tests/test_dataset.py
./bin/unit_tests
```

## Documentation

Update documentation when adding features:
- README.md for user-facing changes
- Doxygen comments for API changes
- docstrings for Python scripts

## License

By contributing, you agree that your contributions will be licensed under the GPL-3.0 license.