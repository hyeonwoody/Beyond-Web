#pragma once
#include <sstream>
#include <mutex>

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
}
#include "Task.h" 
#include <string>


class IMultiMux : public ITask {

protected:
    Kamsi* kamsi;
protected:

    IMultiMux(const std::string& className, TaskType taskType, Kamsi* kamsi)
        : ITask(className, taskType), kamsi(kamsi) {};
public:
    virtual ~IMultiMux() {
        return;
    }
    
};