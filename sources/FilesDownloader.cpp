#include "FilesDownloader.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <regex>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace fs = boost::filesystem;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

std::string getFilename(const std::string& url) {
    return url.substr(url.find_last_of('/') + 1);
}

void download_file(const std::string& url, const std::string& output_directory) {
    boost::asio::io_context ioc;
    tcp::resolver resolver(ioc);
    tcp::socket socket(ioc);

    auto const pos = url.find("://");

    auto host = url.substr(pos + 3);
    auto target = "/";
    auto slashPos = host.find('/');
    if (slashPos != std::string::npos) {
        target = host.substr(slashPos).c_str();
        host = host.substr(0, slashPos).c_str();
    }

    auto const results = resolver.resolve(host, "80");
    boost::asio::connect(socket, results.begin(), results.end());

    http::request<http::empty_body> req{http::verb::get, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);

    boost::beast::flat_buffer buffer;
    http::response<http::file_body> res;

    std::string filename = getFilename(url);
    auto filepath = fs::path(output_directory) / filename;

    boost::beast::error_code ec;
    res.body().open(filepath.string().c_str(), boost::beast::file_mode::write, ec);

    http::read(socket, buffer, res);

    std::cout << "Downloaded" << std::endl;

    socket.shutdown(tcp::socket::shutdown_both);
}

void FilesDownloader::downloadFiles(const std::vector<std::string>& urls, const std::string& output_directory, int max_parallel_downloads) {
    std::queue<std::string> url_queue;
    for (const auto& url : urls) {
        url_queue.push(url);
    }

    std::mutex queue_mutex;

    auto worker = [&]() {
        while (true) {
            std::string url;
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                if (url_queue.empty()) {
                    return; 
                }
                url = url_queue.front();
                url_queue.pop();
            }

            std::cout << "Starting download: " << url << std::endl;
            download_file(url, output_directory);
            std::cout << "Finished download: " << url << std::endl;
        }
    };

    std::vector<std::future<void>> futures;
    for (int i = 0; i < max_parallel_downloads; ++i) {
        futures.push_back(std::async(std::launch::async, worker));
    }

    for (auto& future : futures) {
        future.get();
    }

    std::cout << "All downloads completed!" << std::endl;
}