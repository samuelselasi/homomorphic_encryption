#include "seal/seal.h"
#include <iostream>

using namespace std;
using namespace seal;

int main() {
    // Step 1: Set encryption parameters for BFV
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(4096);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(4096));
    parms.set_plain_modulus(PlainModulus::Batching(4096, 20));

    SEALContext context(parms);
    KeyGenerator keygen(context);

    PublicKey public_key;
    keygen.create_public_key(public_key);
    SecretKey secret_key = keygen.secret_key();

    // Generate Galois keys and store them in a GaloisKeys object (not Serializable)
    GaloisKeys galois_keys;
    keygen.create_galois_keys(galois_keys);  // Corrected: Pass by reference

    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    BatchEncoder batch_encoder(context);

    size_t slot_count = batch_encoder.slot_count();

    // Step 2: Simulate a keyword hit map
    vector<uint64_t> keyword_hits(slot_count, 0);
    keyword_hits[0] = 1;
    keyword_hits[1] = 1;
    keyword_hits[2] = 0;
    keyword_hits[3] = 1;  // Total = 3 hits

    // Step 3: Encrypt the keyword hits
    Plaintext plain_hits;
    batch_encoder.encode(keyword_hits, plain_hits);

    Ciphertext encrypted_hits;
    encryptor.encrypt(plain_hits, encrypted_hits);

    // Step 4: Sum the encrypted slots (simulate reduction)
    Ciphertext total_hits = encrypted_hits;

    for (size_t i = 1; i <= 3; ++i) {
        Ciphertext rotated;
        evaluator.rotate_rows(encrypted_hits, i, galois_keys, rotated);  // Now works
        evaluator.add_inplace(total_hits, rotated);
    }

    // Step 5: Decrypt and decode
    Plaintext result_plain;
    decryptor.decrypt(total_hits, result_plain);

    vector<uint64_t> result;
    batch_encoder.decode(result_plain, result);

    cout << "Total keyword hits (encrypted): " << result[0] << endl;

    return 0;
}
