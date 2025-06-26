#pragma once
#include "MultiMux.h"
#include "Kamsi.h"

class DeMuxTask : public IMultiMux {
private:
    DeMuxTask(TaskType taskType, Kamsi* kamsi) 
    : IMultiMux("DeMuxTask", taskType, kamsi) {};
public:
    static IMultiMux* Create(Kamsi* kamsi);
    ~DeMuxTask();
    void execute() override;
    int ReadFrame(AVFormatContext* ofc, AVPacket* pkt);
    
};
