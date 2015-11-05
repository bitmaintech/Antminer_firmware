#include "get_ip.h"

char *ip_list;

size_t handle_res(void *buffer, size_t size, size_t nmem, void *userp)
{
    ip_list = (char *)calloc(strlen((char *)buffer)+1,sizeof(char));
    strcpy(ip_list,(char *) buffer);
    return size * nmem;
}

char * get_ip_list(char *url)
{
    CURL *curl;
    ip_list = NULL;

    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        curl_global_cleanup();
        return NULL;
    }

    curl = curl_easy_init();

    if (NULL == curl)
    {
        curl_global_cleanup();
        curl_easy_cleanup(curl);
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_res);

    if (curl_easy_perform(curl) != CURLE_OK)
    {
        curl_global_cleanup();
        curl_easy_cleanup(curl);
        return NULL;
    }
    curl_global_cleanup();
    curl_easy_cleanup(curl);
    return ip_list;
}

unsigned char *hex_to_uc(char *hex_string)
{
    int for_tag;
    int len = strlen(hex_string) - 1;
    unsigned char *uc = NULL;

    uc = (unsigned char *) calloc( len / 2,sizeof(unsigned char));
    if (NULL == uc)
    {
        return NULL;
    }

    for (for_tag = 0; for_tag < len; for_tag++)
    {
        if (hex_string[for_tag] >= 'a' && hex_string[for_tag] <= 'z')
            hex_string[for_tag] -= 32;
    }

    for (for_tag = 0; for_tag < len; for_tag += 2)
    {
        if (hex_string[for_tag] >= 'A' && hex_string[for_tag] <= 'F')
            hex_string[for_tag] -= 55;
        else
            hex_string[for_tag] -= 48;

        if (hex_string[for_tag + 1] >= 'A' && hex_string[for_tag + 1] <= 'F')
            hex_string[for_tag + 1] -= 55;
        else
            hex_string[for_tag + 1] -= 48;

        uc[for_tag / 2] = (unsigned char) (((hex_string[for_tag] & 0xf) << 4) | (hex_string[for_tag + 1] & 0xf));
    }
    return uc;
}

