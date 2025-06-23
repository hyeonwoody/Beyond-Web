#pragma once
#include "MediaType.h" 
#include "Task.h" 
#include <map>
#include <string>
#include <functional>
#include <filesystem>
#include <any>
#include <iostream>

namespace fs = std::filesystem;

class IPut : public ITask {
protected:
    std::string url;
    MediaType mediaType;

public:
    IPut(TaskType taskType, const std::string& url, MediaType mediaType)
        : ITask(taskType), url(url), mediaType(mediaType) {}
    virtual const std::string& GetUrl() {return url;}
    virtual MediaType GetMediaType() {return mediaType;}
    virtual const std::string GetPath() {return fs::path(url).parent_path().string();}
    virtual const std::string GetFilename() {return fs::path(url).filename().string();}
    virtual bool Remove() {
        const std::string& inputPath = GetUrl();
        std::error_code ec;
        if (fs::remove(inputPath, ec)) {
            std::cout << "🔊[IPut::Remove] Removed original file: " << inputPath << "\n";
            return true;
        } 
        std::cerr << "🔊[IPut::Remove] Failed to remove original file: " << inputPath
                      << " (" << ec.message() << ")\n";
        return false;
    }
    virtual ~IPut() = default;
};