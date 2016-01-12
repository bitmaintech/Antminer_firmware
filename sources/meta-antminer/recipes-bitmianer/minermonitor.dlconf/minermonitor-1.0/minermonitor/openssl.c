#if 0
#include "openssl.h"
#include "debug.h"

////////////////////////////////////////////////////////////////////////
///////////////////OpenSSL/////////////////////////////////////
char *api_key_send(char *data)
{
    strcpy(data,"api_key=");
    strcat(data,read_luci_conf("api_key"));
    strcat(data,",");
    return data;
}
char *nonce_send(char *data_nance)
{
    char *nonce_tmp;
    strcpy(data_nance,"nonce=");
    nonce_tmp=gen_cuniqid(nonce);
    strcat(data_nance,nonce_tmp);
    strcat(data_nance,",");
    // free(nonce);
    return data_nance;
}
 char *api_key_ssl(char *data,char *result_hex)
{
    // The secret key for hashing

    unsigned char result[67];
    unsigned int len = 67;
    strcpy(data, read_luci_conf("api_key"));
    strcat(data,nonce); DEBUG_printf("api_key_ssl--------------  %s\n",nonce);

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, read_luci_conf("api_security"), strlen(read_luci_conf("api_security")), EVP_sha256(), NULL);
    HMAC_Update(&ctx, (unsigned char*)data, strlen(data));
    HMAC_Final(&ctx, result, &len);
    HMAC_CTX_cleanup(&ctx);
    int i;
    char b[2];
    strcpy(result_hex,"signature=");
    for ( i = 0; i != len; i++)
    {
        //DEBUG_printf("%02x", (unsigned int)result[i]);
	sprintf(b,"%02x",(unsigned int)result[i]);
	strcat(result_hex,b);
    }
    strcat(result_hex,",");
	//DEBUG_printf("result_hex=  %s\n",result_hex);
return result_hex;
}
 #endif

