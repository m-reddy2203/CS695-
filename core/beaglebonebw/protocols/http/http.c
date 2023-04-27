#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <curl/curl.h>
#include "http.h"

CURL *handle;


bool socket_sender(const char* endp, int p, const char* _top, const char* j, long t)
{
    printf("			*** ");
	
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();
    
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");

    if(handle) 
    {
	//curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, j);
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(handle, CURLOPT_URL, endp);
        curl_easy_setopt(handle, CURLOPT_PORT, p);
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, t-3);	
      

        curl_easy_cleanup (handle);
    }
    curl_global_cleanup();
   
    printf("\n");
   
    }

bool init_socket(const char* endp, int p, bool ft_http)
{
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();
    if(handle) 
    {
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);	/* Hide the response */
        curl_easy_setopt(handle, CURLOPT_URL, endp);
        curl_easy_setopt(handle, CURLOPT_PORT, p);
        curl_easy_setopt(handle, CURLOPT_TIMEOUT, 1);
       

        curl_easy_cleanup (handle);
    }
    curl_global_cleanup();

  
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}
