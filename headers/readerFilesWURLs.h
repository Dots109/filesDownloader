#pragma once

#include <vector>
#include <string>

class ReaderFileWURLs {
  public:
    static std::vector<std::string> readURLs(const std::string& file_path);
};