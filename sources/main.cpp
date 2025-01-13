#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <curl/curl.h>
#include "readerFilesWURLs.h"
#include "FilesDownloader.h"

//TODO: Conan, Docker, unit tests, exceptions, spdlog

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << argv[0];
        return 1;
    }

    std::string url_file = argv[1];
    std::string output_directory = argv[2];
    int max_parallel_downloads = std::stoi(argv[3]);

    std::cout << "url_file=" << url_file << " output_directory=" << output_directory 
                << " parallel_downloads=" << max_parallel_downloads << std::endl;

    std::filesystem::create_directories(output_directory);
    
    auto urls = ReaderFileWURLs::readURLs(url_file);
    for(auto &i: urls) {
        std::cout << i << std::endl;
    }

    FilesDownloader::downloadFiles(urls, output_directory, max_parallel_downloads);

}
