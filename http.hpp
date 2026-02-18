
#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <curl/curl.h>
#include <iostream>

using namespace std;

size_t writeCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string httpGet(const string& url) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Enable verbose output
        
        // SSL/TLS Configuration for HTTPS (Linux/WSL with system CA certificates)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // Enable SSL verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); // Verify hostname
        
        // Set timeout to 10 seconds
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cout << "libcurl error: " << curl_easy_strerror(res) << endl;
            response = "";
        }
        curl_easy_cleanup(curl);
    } else {
        cout << "Error: Failed to initialize libcurl" << endl;
    }
    return response;
}

#endif
