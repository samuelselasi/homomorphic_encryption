# Microsoft SEAL Experiments

This directory contains experiments and tests using the 
[Microsoft SEAL](https://github.com/microsoft/SEAL) homomorphic encryption library, 
as part of a thesis exploring secure handling of third-party data in digital forensic 
investigations using homomorphic encryption.


## Purpose

The goal is to understand the capabilities, limitations, and practical 
integration of Microsoft SEAL into a digital forensics workflow. This includes:

- Understanding `BFV` and `CKKS` encryption schemes
- Encrypting and performing arithmetic on encrypted data
- Measuring performance and *computational overhead*
- Simulating simple digital forensic workflows

## Setup

Ensure Microsoft SEAL is installed using `vcpkg`:
```
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
./vcpkg install seal
```

Include the appropriate flags when compiling:
```
g++ your_file.cpp \
  -I<path_to_vcpkg>/installed/x64-linux/include \
  <path_to_vcpkg>/installed/x64-linux/lib/libseal-4.1.a \
  -lz -lzstd \
  -o your_output
```

## File Descriptions

- [seal_hello.cpp](./seal_hello.cpp):			Basic test: encryption, multiplication, decryption using `BFV`

- [seal_batch_test.cpp](./seal_batch_test.cpp):		Tests batching support with `BFV` and demonstrates encoding vectors

- [seal_ckks_test.cpp](./seal_ckks_test.cpp):		Initial test using the `CKKS` scheme for real-number encoding

- [seal_forensics_test.cpp](./seal_forensics_test.cpp):	Simulates a small forensic workflow (e.g., keyword search over encrypted data)

- [seal_utils.h](./seal_utils.h):			Utility functions used across multiple SEAL tests



## [seal_hello.cpp](./seal_hello.cpp)

This program demonstrates a simple example of homomorphic encryption using the `BFV` scheme in Microsoft SEAL. 
It performs basic arithmetic (*multiplication by 2*) on encrypted data, then decrypts and prints the result.


### What It Does

* Initializes encryption parameters for the BFV scheme.

* Uses batching (SIMD-style processing) via BatchEncoder.

* Encrypts the integer 500.

* Multiplies the encrypted value by 2 while it's still encrypted.

* Decrypts the result and prints 1000.

### Why This Matters

This test verifies that SEAL can perform encrypted arithmetic reliably, 
a foundational requirement for applying homomorphic encryption to digital forensic workflows, such as:

* Encrypted metadata aggregation

* Encrypted keyword matching

* Pattern-based evidence scoring

### Key Components Used

* **BFV Scheme**: Supports integer arithmetic on encrypted values.

* **BatchEncoder**: Enables vector-style plaintext encoding, necessary for efficient computation.

* **Evaluator**: Performs operations (here, multiplication) on ciphertexts.
