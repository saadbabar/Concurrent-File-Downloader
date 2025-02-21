// Header file for function headers and definitions

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <mutex>


class Downloader {
    private:
        const std::string url;    // URL of the file to download
        std::string output_file;  // Output file name
        const unsigned int num_threads; // Number of threads to use

        std::string temp_dir;
        std::mutex mtx;

        // Callback function to write data to a file
        static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream){
            return fwrite(ptr, size, nmemb, stream);
        }

        // this function downloads its subsequent part of the file, given where to start and end
        void download_chunk(unsigned long start, unsigned long end, int thread_id);
        void combine_chunks();

    public:
        Downloader(const std::string &url, std::string &output_file, const unsigned int num_threads);
        unsigned long get_file_size();
        void concurrent_download();
        std::vector<std::pair<unsigned long, unsigned long> > divide_bytes_by_thread();
        void change_output_format();
};


#endif