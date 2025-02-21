#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include "downloader.h"


int main(int argc, char *argv[]) {
    // have an optional argument for num_threads -> getopt

    unsigned int num_threads = 1; // default num threads used = 1
    char *url = NULL;
    // char *output_file = NULL;
    std::string output_file;
    int opt;

    if (argc != 3 && argc != 5) {
        std::cerr << "Incorrect amount of arguments passed in" << std::endl;
        std::cerr << "Usage: ./downloader [-t num_threads] <url> <output_file>" << std::endl;
        return 1;
    }

    // getopt stops at the first positional argument
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch(opt) {
            case 't':
                printf("Found -t option. optarg = %s\n", optarg);
                if (atoi(optarg) < 0) {
                    std::cerr << "Negative Threads not possible, try again" << std::endl;
                    return 1;
                    break;
                }
                else if (atoi(optarg) > std::thread::hardware_concurrency()) {
                    std::cout << "Maximum Available Threads: " << std::thread::hardware_concurrency() << std::endl;
                    std::cout << "Setting to Max Threads Available on Machine" << std::endl;
                    num_threads = std::thread::hardware_concurrency();
                }
                else {
                    num_threads = atoi(optarg);
                }
                break;
            case '?':
                std::cerr << "Unknown option passed in" << std::endl;
                return 1;
            default:
                return 1;
        }
    }

    if (optind + 1 > argc) {
        std::cerr << "Missing URL or ouptut file" << std::endl;
    }

    url = argv[optind];
    output_file = argv[optind + 1];

    printf("URL: %s\n", url);
    // change_output_format(url, output_file);
    printf("Output File: %s\n", output_file.c_str());
    printf("Number of Threads: %d\n", num_threads);

    // unsigned long NUM_BYTES = get_file_size(url);

    // concurrent_download(url, num_threads);
    // divide_bytes_by_thread(NUM_BYTES, num_threads);

    Downloader downloader(url, output_file, num_threads);
    downloader.change_output_format();
    downloader.divide_bytes_by_thread();
    downloader.concurrent_download();

    return 0;

}