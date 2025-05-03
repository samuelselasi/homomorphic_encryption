#include "seal/seal.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;
using namespace seal;

// Function to print SEAL parameters
void print_parameters(const SEALContext &context) {
    auto &context_data = *context.key_context_data();
    
    cout << "\n===== Encryption Parameters =====" << endl;
    cout << "Scheme: CKKS" << endl;
    cout << "Poly modulus degree: " << context_data.parms().poly_modulus_degree() << endl;
    
    cout << "Coeff modulus size: ";
    cout << context_data.total_coeff_modulus_bit_count() << " bits (";
    auto coeff_modulus = context_data.parms().coeff_modulus();
    for (size_t i = 0; i < coeff_modulus.size() - 1; i++) {
        cout << coeff_modulus[i].bit_count() << " + ";
    }
    cout << coeff_modulus.back().bit_count() << ")" << endl;
    
    cout << "Scale (2^40): " << pow(2.0, 40) << endl;
    cout << "===============================\n" << endl;
}

int main() {
    try {
        // Step 1: Set up encryption parameters
        EncryptionParameters parms(scheme_type::ckks);
        
        // Recommended parameters for CKKS:
        // - poly_modulus_degree: 8192 (good balance between security and performance)
        // - coeff_modulus: {60, 40, 40, 60} bits provides 128-bit security
        size_t poly_modulus_degree = 8192;
        parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::Create(
            poly_modulus_degree, {60, 40, 40, 60}));
        
        // Create SEAL context and check if parameters are valid
        SEALContext context(parms);
        if (!context.parameters_set()) {
            throw runtime_error("Invalid encryption parameters!");
        }
        
        print_parameters(context);
        
        // Step 2: Generate keys
        KeyGenerator keygen(context);
        
        // Generate public and secret keys
        PublicKey public_key;
        keygen.create_public_key(public_key);
        SecretKey secret_key = keygen.secret_key();
        
        // Generate relinearization keys (needed for multiplication)
        RelinKeys relin_keys;
        keygen.create_relin_keys(relin_keys);
        
        // Generate Galois keys (needed for rotations)
        GaloisKeys galois_keys;
        keygen.create_galois_keys(galois_keys);
        
        // Create helper objects
        Encryptor encryptor(context, public_key);
        Evaluator evaluator(context);
        Decryptor decryptor(context, secret_key);
        CKKSEncoder encoder(context);
        
        // Step 3: Encode and encrypt data
        vector<double> input = {3.5, 7.2, 1.8, 4.3};  // More realistic input size
        double scale = pow(2.0, 40);  // Scale for encoding
        
        cout << "Input data: ";
        for (auto val : input) cout << val << " ";
        cout << endl;
        
        Plaintext plain;
        encoder.encode(input, scale, plain);
        
        Ciphertext encrypted;
        encryptor.encrypt(plain, encrypted);
        cout << "Encryption successful!" << endl;
        
        // Step 4: Perform homomorphic operations
        // Homomorphic addition
        evaluator.add_inplace(encrypted, encrypted);  // Adds encrypted to itself
        
        // Optional: Homomorphic multiplication (requires relinearization)
        // Plaintext plain_two;
        // encoder.encode(2.0, scale, plain_two);
        // evaluator.multiply_plain_inplace(encrypted, plain_two);
        // evaluator.relinearize_inplace(encrypted, relin_keys);
        // evaluator.rescale_to_next_inplace(encrypted);
        
        cout << "Homomorphic operations completed!" << endl;
        
        // Step 5: Decrypt and decode
        Plaintext decrypted_plain;
        decryptor.decrypt(encrypted, decrypted_plain);
        
        vector<double> result;
        encoder.decode(decrypted_plain, result);
        
        // Print results
        cout << "\n===== Results =====" << endl;
        cout << "Expected result (input + input): ";
        for (auto val : input) cout << val + val << " ";
        cout << endl;
        
        cout << "Decrypted result: ";
        for (auto val : result) cout << val << " ";
        cout << endl;
        
        // Calculate and print error (due to encoding/encryption noise)
        cout << "\nApproximation error:" << endl;
        for (size_t i = 0; i < result.size(); i++) {
            double expected = input[i] + input[i];
            double error = abs(result[i] - expected);
            cout << "Element " << i << ": " << error << endl;
        }
        
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
    
    return 0;
}
