#include "seal/seal.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace seal;

// Forensic document structure
struct ForensicDocument {
    int id;
    string encrypted_content;  // In real use, this would be a Ciphertext
    vector<string> keywords;
};

// Helper function to print parameters
void print_parameters(const SEALContext &context) {
    auto &context_data = *context.key_context_data();
    
    cout << "\n===== Forensic Encryption Parameters =====" << endl;
    cout << "Scheme: CKKS (for approximate numeric operations)" << endl;
    cout << "Poly modulus degree: " << context_data.parms().poly_modulus_degree() << endl;
    cout << "Security level: ~128 bits" << endl;
    cout << "=======================================\n" << endl;
}

// Simulates creating a forensic evidence database
vector<ForensicDocument> create_forensic_database() {
    return {
        {1, "encrypted_doc_1", {"murder", "knife", "blood"}},
        {2, "encrypted_doc_2", {"theft", "jewelry", "safe"}},
        {3, "encrypted_doc_3", {"fraud", "documents", "signature"}},
        {4, "encrypted_doc_4", {"murder", "gun", "alibi"}},
        {5, "encrypted_doc_5", {"kidnapping", "ransom", "phone"}}
    };
}

// Encodes and encrypts keywords (simplified for demonstration)
map<string, Ciphertext> encrypt_keywords(const vector<string> &keywords, 
                                       const CKKSEncoder &encoder, 
                                       const Encryptor &encryptor, 
                                       double scale) {
    map<string, Ciphertext> encrypted_keywords;
    
    for (const auto &keyword : keywords) {
        vector<double> values(encoder.slot_count(), 0.0);
        // Simple hash to create a numeric representation
        double hash_val = 0;
        for (char c : keyword) {
            hash_val += c;
        }
        values[0] = hash_val;  // Store in first slot
        
        Plaintext plain;
        encoder.encode(values, scale, plain);
        
        Ciphertext encrypted;
        encryptor.encrypt(plain, encrypted);
        encrypted_keywords[keyword] = encrypted;
    }
    
    return encrypted_keywords;
}

// Homomorphic keyword search
vector<int> search_keyword(const string &query, 
                          const map<string, Ciphertext> &encrypted_keywords,
                          const vector<ForensicDocument> &database,
                          const CKKSEncoder &encoder,
                          const Evaluator &evaluator,
                          Decryptor &decryptor,  // Changed to non-const reference
                          double scale) {
    vector<int> matching_docs;
    
    // In real implementation, this would be encrypted on client side
    double query_hash = 0;
    for (char c : query) {
        query_hash += c;
    }
    
    // Create plaintext query
    vector<double> query_vec(encoder.slot_count(), 0.0);
    query_vec[0] = query_hash;
    Plaintext plain_query;
    encoder.encode(query_vec, scale, plain_query);
    
    // Search each document
    for (const auto &doc : database) {
        for (const auto &kw : doc.keywords) {
            if (!encrypted_keywords.count(kw)) continue;
            
            // Get encrypted keyword
            Ciphertext encrypted_kw = encrypted_keywords.at(kw);
            
            // Create a copy for comparison
            Ciphertext encrypted_diff = encrypted_kw;
            
            // Subtract query from keyword (homomorphic subtraction)
            evaluator.sub_plain_inplace(encrypted_diff, plain_query);
            
            // Check if difference is zero (match)
            Plaintext plain_diff;
            decryptor.decrypt(encrypted_diff, plain_diff);
            
            vector<double> diff_result;
            encoder.decode(plain_diff, diff_result);
            
            // If difference is near zero (accounting for CKKS approximation)
            if (abs(diff_result[0]) < 0.1) {
                matching_docs.push_back(doc.id);
                break;  // No need to check other keywords in this doc
            }
        }
    }
    
    return matching_docs;
}

int main() {
    try {
        // Set up encryption parameters
        EncryptionParameters parms(scheme_type::ckks);
        size_t poly_modulus_degree = 8192;
        parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::Create(
            poly_modulus_degree, {60, 40, 40, 60}));
        
        SEALContext context(parms);
        print_parameters(context);
        
        // Generate keys
        KeyGenerator keygen(context);
        PublicKey public_key;
        keygen.create_public_key(public_key);
        SecretKey secret_key = keygen.secret_key();
        RelinKeys relin_keys;
        keygen.create_relin_keys(relin_keys);
        
        // Create helper objects
        Encryptor encryptor(context, public_key);
        Evaluator evaluator(context);
        Decryptor decryptor(context, secret_key);  // Non-const object
        CKKSEncoder encoder(context);
        double scale = pow(2.0, 40);
        
        // Create forensic database
        vector<ForensicDocument> database = create_forensic_database();
        
        // Extract and encrypt all keywords
        vector<string> all_keywords;
        for (const auto &doc : database) {
            all_keywords.insert(all_keywords.end(), 
                              doc.keywords.begin(), 
                              doc.keywords.end());
        }
        
        // Remove duplicates
        sort(all_keywords.begin(), all_keywords.end());
        all_keywords.erase(unique(all_keywords.begin(), all_keywords.end()), 
                         all_keywords.end());
        
        // Encrypt keywords (would be done by data owner)
        auto encrypted_keywords = encrypt_keywords(all_keywords, encoder, 
                                                 encryptor, scale);
        
        // Interactive search loop
        while (true) {
            cout << "\nForensic Keyword Search (type 'exit' to quit)\n";
            cout << "Available keywords: ";
            for (const auto &kw : all_keywords) {
                cout << kw << " ";
            }
            cout << "\nEnter search term: ";
            
            string query;
            getline(cin, query);
            
            if (query == "exit") break;
            
            // Perform search
            auto results = search_keyword(query, encrypted_keywords, database,
                                        encoder, evaluator, decryptor, scale);
            
            // Display results
            if (results.empty()) {
                cout << "No documents found containing '" << query << "'\n";
            } else {
                cout << "Documents containing '" << query << "': ";
                for (int id : results) {
                    cout << "DOC-" << id << " ";
                }
                cout << endl;
            }
        }
        
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
    
    return 0;
}
