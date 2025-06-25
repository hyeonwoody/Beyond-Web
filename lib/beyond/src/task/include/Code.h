#pragma once
#include <sstream>
#include <mutex>

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
}
#include "Task.h" 
#include <string>
enum class CodeType {
    UNKNOWN,
    HLS,
};

class ICode : public ITask {
protected:
    std::string url;
    std::mutex logMutex;
    std::ostringstream logOutput;
    ICode(const std::string& className, TaskType taskType)
        : ITask(className, taskType) {};
public:

    AVFormatContext* formatContext = nullptr;
    virtual ~ICode() {
        return;
    }
    virtual std::string DumpLog( AVFormatContext* formatContext, int index, const std::string& url, int isOutput) {
        {
            std::lock_guard<std::mutex> lock(logMutex);
            logOutput.str("");
            logOutput.clear();
        }
        formatContext->opaque = this;
        av_log_set_callback(ICode::LogCallback);
        av_dump_format(formatContext, index, url.c_str(), isOutput);
        std::lock_guard<std::mutex> lock(logMutex);
        return logOutput.str();
    }
    static void LogCallback(void* ptr, int level, const char* fmt, va_list vl) {
        auto* fmtCtx = static_cast<AVFormatContext*>(ptr);
        if (!fmtCtx || !fmtCtx->opaque) return;
        auto* self = static_cast<ICode*>(fmtCtx->opaque);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), fmt, vl);

        std::lock_guard<std::mutex> lock(self->logMutex);
        self->logOutput << buffer;
    }
    virtual int GetStreamInfo() = 0;
};