#include "downloader.h"
#include <curl/curl.h>
#include <iostream>

long get_file_size(const std::string& url) {
    // Use curl to get HTTP header which has content-length
    // create a curl session
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl failed to execute\n");
        curl_global_cleanup(); // Clean up instance of libcurl
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // Prevent downloading file contents
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L); // Return error for 4xx/5xx responses

    CURLcode res = curl_easy_perform(curl);
    double content_length = 0;
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length);
    }
    curl_easy_cleanup(curl);

    if (content_length > 0) {
        return static_cast<long>(content_length);
    }
    return -1;
}

void concurrent_download(const std::string &url, unsigned int num_threads) {
    long file_size = get_file_size(url);

    if (file_size <= 0) {
        std::cerr << "Failed to determine File Size of Download" << std::endl;
        return;
    }

    std::cout << "Downloading " << file_size << " bytes using " << num_threads << " threads..." << std::endl;
}