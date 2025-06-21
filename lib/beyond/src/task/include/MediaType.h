#pragma once
#include <string>
#include <utility>
enum class MediaType {
    UNKNOWN,
    FILE,
    HTTP,
};

inline std::pair<MediaType, std::string> parseMediaType(const std::string& url) {
    const std::string filePrefix = "file://";
    const std::string httpPrefix = "http://";

    if (url.compare(0, filePrefix.size(), filePrefix) == 0) {
        return { MediaType::FILE, url.substr(filePrefix.size()) };
    }
    else if (url.compare(0, httpPrefix.size(), httpPrefix) == 0) {
        return { MediaType::HTTP, url.substr(httpPrefix.size()) };
    }
    else {
        return { MediaType::UNKNOWN, url };
    }
}