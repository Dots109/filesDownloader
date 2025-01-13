#pragma once

#include <iostream>
#include <vector>
#include <string>

class FilesDownloader {
  public:
    static void downloadFiles(const std::vector<std::string>& urls, const std::string& output_directory, int max_parallel_downloads);
};

