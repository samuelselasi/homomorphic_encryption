# Basic Examples

## File Descriptions

- [seal_hello.cpp](./seal_hello.cpp):                   Basic test: encryption, multiplication, decryption using `BFV`

- [seal_batch_test.cpp](./seal_batch_test.cpp):         Tests batching support with `BFV` and demonstrates encoding vectors

- [seal_ckks_test.cpp](./seal_ckks_test.cpp):           Initial test using the `CKKS` scheme for real-number encoding

- [seal_forensics_test.cpp](./seal_forensics_test.cpp): Simulates a small forensic workflow (e.g., keyword search over encrypted data)

- [seal_utils.h](./seal_utils.h):                       Utility functions used across multiple SEAL tests



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

## [seal_batch_test.cpp](./seal_batch_test.cpp)


### Description

This example demonstrates the basic use of batch encoding in the `BFV` scheme
using Microsoft SEAL. It encodes a vector of integers, encrypts them, performs
a simple homomorphic operation (multiplication by a plaintext scalar),
and decrypts the result.

This test helps verify that batching is correctly enabled and functioning,
serving as a foundational step for more advanced operations like
`slot-wise` processing in encrypted data — which is crucial in
digital forensics workflows involving evidence scoring or filtering.

### Features Demonstrated

* Use of the `BFV` encryption scheme

* Batch encoding of a vector using `BatchEncoder`

* Encryption and decryption using `Encryptor` and `Decryptor`

* Homomorphic multiplication of encrypted data by a plaintext scalar

* Full homomorphic computation cycle: `encode →  encrypt →  evaluate →  decrypt →  decode`


### Relevance to Digital Forensics

This test simulates a simplified scenario where a value
(e.g., an evidence weight or score) is encrypted,
homomorphically manipulated, and then decrypted.

In forensic workflows, such computations could be used to
securely apply scoring rules or filters to encrypted data
without exposing the underlying evidence.

### Output Example

```
Total keyword hits (encrypted): 3
```



## [seal_ckks_test.cpp](./seal_ckks_test.cpp)

### Description

This program is a basic demonstration of Microsoft SEAL's CKKS scheme,
which enables approximate arithmetic over real numbers using homomorphic encryption.
The test encrypts a vector of real numbers, performs homomorphic addition
(doubling the vector), then decrypts and compares the results to the expected plaintext values.

***It showcases***:

* Setting up encryption parameters for `CKKS`.

* Encoding and encrypting real numbers.

* Performing encrypted addition.

* Decrypting and decoding results.

* Calculating approximation errors introduced by the encryption scheme.

### Purpose

This test is part of a larger thesis project exploring the
applicability of Homomorphic Encryption in Digital Forensics workflows.
