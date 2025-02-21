// Header file for function headers and definitions

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>

long get_file_size(const std::string& url);
void concurrent_download(const std::string& url, unsigned int num_threads);


#endif