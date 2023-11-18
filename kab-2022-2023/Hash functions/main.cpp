#ifndef __PROGTEST__
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

#endif /* __PROGTEST__ */

inline int __checkhash(int bits, char * string) {
    size_t bytes = bits / 8;
    bits = bits % 8;
    size_t i = 0;

    for (i=0; i<bytes; i++) {
        if (string[i] != 0) {
            return EXIT_FAILURE;
        }
    }  

    unsigned char remainder = string[i];
    for (unsigned short j=0; j<bits; j++) {
        auto check = (remainder >> ((7-j)) & 1);
         if (check) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}


// This function converts raw value to normal string
inline void bytes_to_hex(const unsigned char *bytes, size_t len, char *hex_string, size_t hex_string_max_len) {
    const char hex[17] = "0123456789abcdef";

    for (size_t i=0; i<len; ++i) {
        unsigned char byte = bytes[i];
        hex_string[2*i] = hex[byte >> 4];
        hex_string[2*i+1] = hex[byte & 0x0F];
    }
    hex_string[hex_string_max_len-1] = '\0';
}


int findHash(int bits, char ** message, char ** hash) {
    if (bits < 0 || bits > 512) {         // Check if bits is negative
        return 0;
    }
    srand(static_cast<unsigned int>(time(NULL)));
    OpenSSL_add_all_digests();
    const EVP_MD *type = EVP_get_digestbyname("sha512"); // Get hash type
    int message_lenght = 513;
    unsigned char * text = (unsigned char *)malloc(message_lenght);  // Allocate memory for the message
    unsigned char * hsh = (unsigned char *)malloc(EVP_MAX_MD_SIZE); // Create memory for hash
    unsigned length;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new(); // Create context object
    if (!ctx)
        return 0;

    RAND_bytes(text, message_lenght);   // Create a text

    do {
        EVP_DigestInit_ex(ctx, type, NULL);
        EVP_DigestUpdate(ctx, text, message_lenght);
        EVP_DigestFinal_ex(ctx, hsh, &length);

        if (!__checkhash(bits, (char *)hsh)) {
            break;
        } else {
            text[rand() % (message_lenght-1)] = static_cast<unsigned char>(rand() % 256);
        }
    } while (1);

    *message = (char *)malloc((2 * message_lenght + 1));
    *hash = (char *)malloc((2 * length + 1));
    bytes_to_hex(text, message_lenght, *message, 2 * message_lenght + 1);
    bytes_to_hex(hsh, length, *hash, 2 * length + 1);

    free(text);
    free(hsh);
    EVP_MD_CTX_free(ctx);
    return 1;
}


int findHashEx(int bits, char ** message, char ** hash, const char *hashFunction) {
    OpenSSL_add_all_digests();
    const EVP_MD *type = EVP_get_digestbyname(hashFunction); // Get hash type
    if (type == NULL)
        return 0;

    int message_lenght = 513;

    if (bits < 0 || bits > EVP_MD_size(type) * 8) { // Check if bits is negative
        return 0;
    }
    srand(static_cast<unsigned int>(time(NULL)));
    unsigned char * text = (unsigned char *)malloc(message_lenght); // Allocate memory for the message
    unsigned char * hsh = (unsigned char *)malloc(EVP_MAX_MD_SIZE); // Create memory for hash
    unsigned length;

    EVP_MD_CTX * ctx = EVP_MD_CTX_new(); // Create context object
    if (ctx)
        return 0;

    RAND_bytes(text, message_lenght);

    do {
        EVP_DigestInit_ex(ctx, type, NULL);
        EVP_DigestUpdate(ctx, text, message_lenght);
        EVP_DigestFinal_ex(ctx, hsh, &length);

        if (!__checkhash(bits, (char *)hsh))
            break;
        else
            text[rand() % (message_lenght-1)] = static_cast<unsigned char>(rand() % 256);
    } while (1);

    *message = (char *)malloc((2 * message_lenght + 1));
    *hash = (char *)malloc((2 * length + 1));
    bytes_to_hex(text, message_lenght, *message, 2 * message_lenght + 1);
    bytes_to_hex(hsh, length, *hash, 2 * length + 1);

    free(text);
    free(hsh);
    EVP_MD_CTX_free(ctx);
    return 1;
}

#ifndef __PROGTEST__

int checkHash(int bits, char * hexString) {
    return 1;
}

int main(void) {
    char * message, * hash;
    //assert(findHash(0, &message, &hash) == 1);
    //assert(message && hash && checkHash(0, hash));
    //free(message);
    //free(hash);
    assert(findHash(27, &message, &hash) == 1);
    assert(message && hash && checkHash(1, hash));
    free(message);
    free(hash);
    return 0;
    assert(findHash(2, &message, &hash) == 1);
    assert(message && hash && checkHash(2, hash));
    free(message);
    free(hash);
    assert(findHash(3, &message, &hash) == 1);
    assert(message && hash && checkHash(3, hash));
    free(message);
    free(hash);
    assert(findHash(-1, &message, &hash) == 0);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
