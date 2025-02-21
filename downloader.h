// Header file for function headers and definitions

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>

unsigned long get_file_size(const std::string& url);
void concurrent_download(const std::string& url, unsigned int num_threads);
void divide_bytes_by_thread(const unsigned long file_size, unsigned int num_threads);

void change_output_format(const std::string& url, std::string& output_file);


#endif