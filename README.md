# RISC-V ELF Decoder

A simple C++ tool to read RISC-V ELF files and decode their `.text` section instructions. This project was developed as part of the Si-Vision assessment.

## Project Structure
- `src/`: Contains the core source code for reading ELF files and decoding RISC-V instructions.
- `tests/`: Contains Google Test unit tests for the decoder logic.
- `lit_tests/`: Contains LLVM LIT regression tests.
- `.github/workflows/`: Contains the CI/CD pipeline configuration for GitHub Actions.

## Requirements Satisfied
1. **ELF Decoder**: Parses 64-bit ELF headers, extracts the `.text` section, and disassembles basic RISC-V instructions (supports Base Integer RV64I like ADDI, ADD, Branches, Load/Store).
2. **Development**: Uses a simple `Makefile` for compilation without complex build systems.
3. **Testing**: 
   - Unit tests implemented using Google Test (`gtest`).
   - Regression tests implemented using `LIT`.
4. **CI/CD**: GitHub Actions workflow automatically builds the project and runs both unit and regression tests on every push.

## How to Build
To build the project on Linux or WSL:
```bash
make
```
This will generate the `decoder` executable.

## How to Run
```bash
./decoder <path_to_elf_file>
```
*Note: A Python script `create_test_elf.py` is included to generate a dummy 64-bit RISC-V ELF file for quick testing.*

## Running Tests
**Unit Tests (Google Test):**
```bash
make test
```
**Regression Tests (LIT):**
```bash
lit lit_tests/
```
