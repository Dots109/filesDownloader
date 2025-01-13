#include "readerFilesWURLs.h"
#include <fstream>
#include <stdexcept>

std::vector<std::string> ReaderFileWURLs::readURLs(const std::string& file_path) {
    std::ifstream file(file_path);

    std::vector<std::string> urls;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            urls.push_back(line);
        }
    }

    return urls;
}
