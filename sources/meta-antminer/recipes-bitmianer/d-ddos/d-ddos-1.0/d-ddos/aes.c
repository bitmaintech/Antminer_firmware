#include "aes.h"

bool aes_encrypt(struct ip_string *ip_c, int e_or_d)
{
    AES_KEY aes;
    unsigned char key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    unsigned char iv[AES_BLOCK_SIZE];        // init vector
    unsigned int for_tag;

    for (for_tag = 0; for_tag < 16; ++for_tag)
    {
        key[for_tag] = 32 + for_tag;
    }

    for (for_tag = 0; for_tag < AES_BLOCK_SIZE; ++for_tag)
    {
        iv[for_tag] = 0;
    }

    if (AES_ENCRYPT == e_or_d)
    {
        ip_c->len = 0;
        if ((strlen((char *) ip_c->source_string) + 1) % AES_BLOCK_SIZE == 0)
        {
            ip_c->len = strlen((char *) ip_c->source_string) + 1;
        } else
        {
            ip_c->len = ((strlen((char *) ip_c->source_string) + 1) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
        }

        if (AES_set_encrypt_key(key, 128, &aes) < 0)
        {
            return false;
        }

        ip_c->encrypt_string = (unsigned char *)calloc(ip_c->len, sizeof(unsigned char));
        if (ip_c->encrypt_string == NULL)
        {
            return false;
        }

        AES_cbc_encrypt(ip_c->source_string, ip_c->encrypt_string, ip_c->len, &aes, iv, AES_ENCRYPT);
    } else
    {

        ip_c->source_string = (unsigned char *)calloc(ip_c->len, sizeof(unsigned char));
        if (ip_c->source_string == NULL)
        {
            return false;
        }

        if (AES_set_decrypt_key(key, 128, &aes) < 0)
        {
            return false;
        }

        AES_cbc_encrypt(ip_c->encrypt_string, ip_c->source_string, ip_c->len, &aes, iv, AES_DECRYPT);
    }
    return true;
}

