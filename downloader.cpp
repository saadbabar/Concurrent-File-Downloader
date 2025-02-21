#include "downloader.h"
#include <curl/curl.h>
#include <iostream>

unsigned long get_file_size(const std::string& url) {
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
        return static_cast<unsigned long>(content_length);
    }
    return -1;
}

void concurrent_download(const std::string &url, unsigned int num_threads) {
    // passing in &url because we want it to modify the original
    long file_size = get_file_size(url);

    if (file_size <= 0) {
        std::cerr << "Failed to determine File Size of Download" << std::endl;
        return;
    }

    std::cout << "Downloading " << file_size << " bytes using " << num_threads << " threads..." << std::endl;
}

void divide_bytes_by_thread(const unsigned long file_size, unsigned int num_threads) {
    // divide which thread gets how many bytes
    unsigned long base_size = file_size/num_threads;
    unsigned long remainder = file_size % num_threads;

    for (unsigned int i = 0; i < num_threads; i++) {
        unsigned long size_of_thread = base_size;
        if (i < remainder) {
            size_of_thread++;
        }
        std::cout << "Thread " << i + 1 << " processing " << base_size << " bytes...\n";
    }

}

void change_output_format(const std::string &url, std::string &output_file) {
    // change file format to the one in the url
    // CHANGES ORIGINAL URL
    // output -> output.pdf or output.html -> output.txt depending on ending of url
    std::vector<char> url_file_format;

    for (int i = url.length() - 1; i >= 0; --i) {
        if (url[i]  == '.') {
            break;
        }
        url_file_format.push_back(url[i]);
    }
    std::reverse(url_file_format.begin(), url_file_format.end());

    std::string new_extension = ".";
    
    for (int i = 0; i < url_file_format.size(); i++) {
        new_extension += url_file_format[i];
    }
    std::cout << "New file format: " << new_extension << std::endl;

    size_t dot_index = output_file.rfind('.');

    if (dot_index != std::string::npos) {
        output_file.replace(dot_index, std::string::npos, new_extension);
    }
    else {
        output_file += new_extension;
    }

}
