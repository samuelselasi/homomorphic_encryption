# Homomorphic Encryption Forensic Analysis with Microsoft SEAL

## Overview
This repository contains my thesis research on applying Microsoft SEAL's homomorphic encryption schemes to digital forensic investigations. It implements:
- Encrypted keyword search over forensic evidence
- Privacy-preserving evidence analysis
- Performance benchmarks for CKKS/BFV schemes
- Reusable SEAL utilities for academic research

## Key Features

| Component            | Description                              | Tech Specs                  |
|----------------------|------------------------------------------|-----------------------------|
| Forensic Search      | Encrypted term matching in documents     | CKKS, 128-bit security      |
| Evidence Analysis    | Secure computation on encrypted case files | BFV, Poly deg 8192          |
| Benchmarks           | Operation timing/memory metrics          | Google Benchmark            |
| SEAL Utilities       | Common encryption workflows              | C++17, CMake                |


## Installation

### Prerequisites
- SEAL 4.1+
- CMake 3.12+
- C++17 compiler

### Build Instructions

```
bash
```
## Clone with submodules

```
git clone --recursive https://github.com/samuelselasi/homomorphic_encryption.git
cd homomorphic_encryption/microsoft_seal
```

# Configure

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```
# Build
```
make -j$(nproc)
```

# Run tests
```
ctest --output-on-failure
```
