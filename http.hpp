
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
