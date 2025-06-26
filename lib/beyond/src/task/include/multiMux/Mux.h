#pragma once
#include "MultiMux.h"
#include "Kamsi.h"

class MuxTask : public IMultiMux {

private:
    MuxTask(TaskType taskType, Kamsi* kamsi) 
    : IMultiMux("MuxTask", taskType, kamsi) {};
public:
    static IMultiMux* Create(Kamsi* kamsi);
    ~MuxTask();
    void execute() override;
    void WriteHeader(AVFormatContext* outputFormatCtx, AVDictionary** options);
    void WriteFrame(AVFormatContext* ofc, AVPacket* pkt);
    void WriteTrailer(AVFormatContext* ofc);
};
