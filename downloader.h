// Header file for function headers and definitions

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>


class Downloader {
    private:
        const std::string url;    // URL of the file to download
        std::string output_file;  // Output file name
        const unsigned int num_threads; // Number of threads to use

    public:
        Downloader(const std::string &url, std::string &output_file, const unsigned int num_threads);
        unsigned long get_file_size();
        void concurrent_download();
        void divide_bytes_by_thread();
        void change_output_format();
};


#endif