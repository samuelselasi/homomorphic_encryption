#include "seal/seal.h"
#include <iostream>

using namespace std;
using namespace seal;

int main() {
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(4096);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(4096));
    parms.set_plain_modulus(PlainModulus::Batching(4096, 20));

    SEALContext context(parms);
    KeyGenerator keygen(context);

    // Generate public and secret keys
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

    BatchEncoder batch_encoder(context);

    // Encode the value
    Plaintext plain;
    batch_encoder.encode(vector<uint64_t>(1, 500), plain);

    Ciphertext encrypted;
    encryptor.encrypt(plain, encrypted);

    cout << "Encrypted!" << endl;

    // Multiply by 2
    Plaintext plain_two;
    batch_encoder.encode(vector<uint64_t>(1, 2), plain_two);
    evaluator.multiply_plain_inplace(encrypted, plain_two);

    Plaintext decrypted;
    decryptor.decrypt(encrypted, decrypted);

    // Decode the value
    vector<uint64_t> decoded;
    batch_encoder.decode(decrypted, decoded);
    cout << "Decrypted value: " << decoded[0] << endl;

    return 0;
}
