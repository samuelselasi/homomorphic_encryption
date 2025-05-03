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

***Specifically, this file***:

* Verifies `CKKS` support in Microsoft SEAL.

* Demonstrates `real-number` support (as opposed to only integers).

* Tests basic operations that would be relevant in forensic metrics 
(e.g., aggregating evidence scores, similarity rankings, etc.).

### Key Operations

* CKKS Setup with parameters {60, 40, 40, 60} for 128-bit security.

* Encoding a vector: [3.5, 7.2, 1.8, 4.3].

* Homomorphic Addition (doubles each number).

* Decryption & Decoding back to floating-point results.

* Error Measurement to quantify the loss due to approximate encryption.

### Sample Output

```
Input data: 3.5 7.2 1.8 4.3 
Encryption successful!
Homomorphic operations completed!

===== Results =====
Expected result (input + input): 7 14.4 3.6 8.6 
Decrypted result: 7.0001 14.3998 3.5999 8.5999 

Approximation error:
Element 0: 0.0001
Element 1: 0.0002
Element 2: 0.0001
Element 3: 0.0001
```


## [seal_forensics_test.cpp](./seal_forensics_test.cpp)

### Description

This program demonstrates a proof-of-concept forensic keyword search system using 
homomorphic encryption (*CKKS scheme*) with Microsoft SEAL. 
It simulates searching for forensic-relevant keywords (e.g., "murder", "fraud") 
across a collection of encrypted documents—without revealing the actual search query or keyword values.


### Goals

* Show how encrypted keywords can be searched homomorphically.

* Demonstrate a basic forensic use case with encrypted metadata.

* Provide a privacy-preserving approach to keyword matching.

* Serve as a sandbox for refining ideas in the thesis.

### How It Works

* **Database Simulation**: A sample database of ForensicDocument structs is created, 
each with mock encrypted content and a list of keywords.

* **Keyword Encryption**: Keywords are converted into numeric hashes and 
encoded/encrypted using `CKKS`.

* **Search Query**: A user inputs a keyword. This is also encoded and used 
for homomorphic comparison.

* **Search Logic**: The system checks for encrypted matches via homomorphic 
subtraction and decrypts only the result of that comparison (not the content or keywords).

* **Results**: Matching document IDs are shown to the user.


### Key Features

| **Component**                 | **Description**                                                                 |
|------------------------------|---------------------------------------------------------------------------------|
| **Scheme**                   | CKKS (supports real-number encryption with approximate arithmetic)              |
| **Keyword Encoding**         | Character-sum hash encoded into the first slot of a CKKS plaintext vector       |
| **Homomorphic Operation**    | Subtraction used to compare encrypted keyword and encrypted query               |
| **Forensic Use-Case Simulated** | Searching encrypted metadata for evidence-related keywords                   |
| **Security Model**           | Only the difference result is decrypted; keywords and queries remain encrypted |


### Sample Output

```
===== Forensic Encryption Parameters =====
Scheme: CKKS (for approximate numeric operations)
Poly modulus degree: 8192
Security level: ~128 bits
=======================================

Forensic Keyword Search (type 'exit' to quit)
Available keywords: alibi blood documents fraud gun jewelry kidnapping knife murder phone ransom safe signature theft 
Enter search term: fraud
Documents containing 'fraud': DOC-3

```


### Limitations

* **Keyword Hashing**: The use of simple *character-sum* hashing is insecure in real systems.

* **Content Encryption**: This demo does not actually encrypt document content — 
only keyword metadata is handled securely.

* **Client/Server Separation**: All operations happen locally; no client-server separation is modeled yet.


### Relevance to Thesis

This example directly supports the goal of **preserving third-party privacy** 
in forensic searches. Only the presence of a keyword match is revealed — 
**not the keywords themselves or unrelated document content**.
