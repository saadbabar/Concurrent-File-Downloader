#include "downloader.h"
#include <curl/curl.h>
#include <iostream>


Downloader::Downloader(const std::string& url, std::string& output_file, unsigned int num_threads)
    : url(url), output_file(output_file), num_threads(num_threads) {
        std::cout << "Downloading " << url << " into " << output_file << " with " << num_threads << " threads..." << std::endl;
    };

unsigned long Downloader::get_file_size() {
    // Use curl to get HTTP header which has content-length
    // create a curl session
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl failed to execute\n");
        curl_global_cleanup(); // Clean up instance of libcurl
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
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

void Downloader::concurrent_download() {
    // passing in &url because we want it to modify the original
    unsigned long file_size = get_file_size();

    if (file_size <= 0) {
        std::cerr << "Failed to determine File Size of Download" << std::endl;
        return;
    }

    std::cout << "Downloading " << file_size << " bytes using " << this->num_threads << " threads..." << std::endl;

}

std::vector<std::pair<unsigned long, unsigned long> > Downloader::divide_bytes_by_thread() {
    // divide which thread gets how many bytes

    std::vector<std::pair<unsigned long, unsigned long> > chunks;
    chunks.resize(this->num_threads);

    const unsigned long file_size = get_file_size();


    unsigned long chunk_size = file_size/this->num_threads;
    unsigned long remainder = file_size % this->num_threads;
    unsigned long start = 0;

    for (unsigned int i = 0; i < num_threads; i++) {
        unsigned long end = start + chunk_size - 1;
        if (i < remainder) {
            end++;
        }
        std::cout << "Thread " << i + 1 << " processing " << start << " to " << end << " bytes...\n";
        chunks[i] = std::make_pair(start, end);
        start = end + 1;
    }

    if (!chunks.empty()) {
        chunks.back().second = file_size - 1;
    }

    return chunks;

}

void Downloader::change_output_format() {
    // change file format to the one in the url
    // CHANGES ORIGINAL URL
    // output -> output.pdf or output.html -> output.txt depending on ending of url
    std::vector<char> url_file_format;

    for (int i = this->url.length() - 1; i >= 0; --i) {
        if (this->url[i]  == '.') {
            break;
        }
        url_file_format.push_back(this->url[i]);
    }
    std::reverse(url_file_format.begin(), url_file_format.end());

    std::string new_extension = ".";
    
    for (int i = 0; i < url_file_format.size(); i++) {
        new_extension += url_file_format[i];
    }
    std::cout << "New file format: " << new_extension << std::endl;

    size_t dot_index = this->output_file.rfind('.');

    if (dot_index != std::string::npos) {
        this->output_file.replace(dot_index, std::string::npos, new_extension);
    }
    else {
        this->output_file += new_extension;
    }
    std::cout << "New file output file name: " << this->output_file << std::endl;
}

void Downloader::download_chunk(unsigned long start, unsigned long end, int thread_id) {
    CURL* curl = curl_easy_init(); // declares a curl object type that will do the work for us

    if (!curl) {
        std::cerr << "Thread " << thread_id << " failed to initialize cURL object" << std::endl;
        return;
    }

    // Create temporatu file to store data in
    std::string chunk_file = output_file + "_chunk_" + std::to_string(thread_id) + ".tmp";
    FILE* file = fopen(chunk_file.c_str(), "wb"); // open chunk file that we created with write permissions

    if (!file) {
        std::cerr << "Thread " << thread_id <<  "failed to open file " << chunk_file << std::endl;
        curl_easy_cleanup(curl);
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    std::string range = std::to_string(start) + "-" + std::to_string(end);
    curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());

    CURLcode res = curl_easy_perform(curl);
    fclose(file);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Thread " << thread_id << " failed to download chunk: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        std::cout << "Thread " << thread_id << " downloaded bytes " << start << " to " << end << std::endl;
    }
}