
#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

struct crypto_config {
	const char * m_crypto_function;
	std::unique_ptr<uint8_t[]> m_key;
	std::unique_ptr<uint8_t[]> m_IV;
	size_t m_key_len;
	size_t m_IV_len;
};

#endif /* _PROGTEST_ */

inline bool execute(const std::string & in_filename, const std::string & out_filename, crypto_config & config, bool to_encrypt) noexcept {
    std::ifstream infile (in_filename, std::ios::binary);       //Opening files
    std::ofstream outfile (out_filename, std::ios::binary);

    if (!infile.is_open() or !outfile.is_open()) {              //Check if opened
        return false;
    }

    const EVP_CIPHER * cipher = NULL;                           //Prepare stuff
    if (config.m_crypto_function) {
        cipher = EVP_get_cipherbyname(config.m_crypto_function);
    }

    if (cipher == NULL) {                       
        return false;
    }

    std::unique_ptr<uint8_t[]> & parameter_iv = config.m_IV;                        //Init-vector config paramenter
    std::unique_ptr<uint8_t[]> & parameter_key = config.m_key;                      //Key config parameter
     __SIZE_TYPE__ & parameter_iv_length = config.m_IV_len;                 //Init-vector parameter length
     __SIZE_TYPE__ & parameter_key_length = config.m_key_len;               //Key parameter lenght
    __SIZE_TYPE__ evp_cipher_parameter_iv_length = EVP_CIPHER_iv_length(cipher);    //Init-vector cipher parameter
    __SIZE_TYPE__ evp_cipher_parameter_key_lenght = EVP_CIPHER_key_length(cipher);  //Key cipher parameter length

    if (EVP_CIPHER_mode(cipher) != EVP_CIPH_CBC_MODE) {
        
    } else if ((parameter_iv == nullptr or parameter_iv_length < evp_cipher_parameter_iv_length)) { // Check if parameters set right
        if (to_encrypt == true) {
            parameter_iv.reset(new uint8_t[evp_cipher_parameter_iv_length]);       // Reset parameters
            if (!RAND_bytes(parameter_iv.get(), evp_cipher_parameter_iv_length)) { // Generate new and check
                return false;
            }
            parameter_iv_length = evp_cipher_parameter_iv_length;
        } else {
            return false;
        }
    }

    if (parameter_key == nullptr or parameter_key_length < evp_cipher_parameter_key_lenght) {
        if (to_encrypt) {
            parameter_key.reset(new uint8_t[evp_cipher_parameter_key_lenght]); // Same bullshit
            if (!RAND_bytes(parameter_key.get(), evp_cipher_parameter_key_lenght)) {
                return false;
            }
            parameter_key_length = evp_cipher_parameter_key_lenght;
        } else {
            return false;
        }
    }

    char h[18];             // Local buffer
    if (!infile.read(h, 18) or !outfile.write(h, 18)) { //try to read/write for the buffer
        return false;
    }

    EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();       //Setup context
    if (ctx == NULL) {
        return false;
    }

    if (to_encrypt) {   
        if (EVP_EncryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
    } else {
        if (EVP_DecryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
    }

    //What the fuck I stopped here, anyway, come back later

    int cipher_block_size = EVP_CIPHER_block_size(cipher);  //Allocate more stuff for the important job
    uint8_t * inbuff = reinterpret_cast<uint8_t*>(malloc(sizeof(uint8_t) * cipher_block_size));
    uint8_t * outbuff = reinterpret_cast<uint8_t*>(malloc(sizeof(uint8_t) * (cipher_block_size + EVP_MAX_BLOCK_LENGTH)));
    int encrypted_bytes = 0;
    int readable_bytes = 0;

    while (true) {  // I hate myself loop
        readable_bytes = infile.read(reinterpret_cast<char*>(inbuff), cipher_block_size).gcount();  //read incoming
        if (readable_bytes <= 0) {
            break;
        }

        if (to_encrypt) {   //Depending on what we want we do what we want to achieveme what we want to get.
            if (EVP_EncryptUpdate(ctx, outbuff, &encrypted_bytes, inbuff, readable_bytes) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                free(inbuff);
                free(outbuff);
                return false;
            }
        } else {
            if (EVP_DecryptUpdate(ctx, outbuff, &encrypted_bytes, inbuff, readable_bytes) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                free(inbuff);
                free(outbuff);
                return false;
            }
        } // STOPPED HERE

        if (!outfile.write(reinterpret_cast<char*>(outbuff), encrypted_bytes)) { //Also important shit to write data
            EVP_CIPHER_CTX_free(ctx);
            free(inbuff);
            free(outbuff);
            return false;
        }
    }
    // You went to eat, stopped here 
    if (to_encrypt) {
        if (EVP_EncryptFinal_ex(ctx, outbuff, &encrypted_bytes) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            free(inbuff);
            free(outbuff);
            return false;
        }
    } else {
        if (EVP_DecryptFinal_ex(ctx, outbuff, &encrypted_bytes) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            free(inbuff);
            free(outbuff);
            return false;
        }
    }

    if (!outfile.write(reinterpret_cast<char*>(outbuff), encrypted_bytes)) { //Check if we couldn't write stuff
        EVP_CIPHER_CTX_free(ctx);
        free(inbuff);
        free(outbuff);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    free(inbuff);
    free(outbuff);

    return true;
}

bool encrypt_data(const std::string &in_filename, const std::string &out_filename, crypto_config &config) {
    return execute(in_filename, out_filename, config, true);
}

bool decrypt_data(const std::string & in_filename, const std::string & out_filename, crypto_config & config) {
    return execute(in_filename, out_filename, config, false);
}


#ifndef __PROGTEST__

bool compare_files ( const char * name1, const char * name2 ) noexcept {
    return true;
}

int main ( void )
{
	crypto_config config {nullptr, nullptr, nullptr, 0, 0};

	// ECB mode
	config.m_crypto_function = "AES-128-ECB";
	config.m_key = std::make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_ecb.TGA") );

	assert( decrypt_data  ("UCM8_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_1_enc_ecb.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_2_enc_ecb.TGA") );

	assert( decrypt_data ("image_3_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_3_dec_ecb.TGA") );

	assert( decrypt_data ("image_4_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_4_dec_ecb.TGA") );

	// CBC mode
	config.m_crypto_function = "AES-128-CBC";
	config.m_IV = std::make_unique<uint8_t[]>(16);
	config.m_IV_len = 16;
	memset(config.m_IV.get(), 0, 16);

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_cbc.TGA") );

	assert( decrypt_data  ("UCM8_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_cbc.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_5_enc_cbc.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_6_enc_cbc.TGA") );

	assert( decrypt_data ("image_7_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_7_dec_cbc.TGA") );

	assert( decrypt_data ("image_8_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_8_dec_cbc.TGA") );
	return 0;
}

#endif /* _PROGTEST_ */
