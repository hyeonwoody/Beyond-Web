#pragma once
#include "Code.h"
#include "Kamsi.h"

class EncodeTask : public ICode {
private:
    Kamsi* kamsi;
private:
    EncodeTask(TaskType taskType) 
    : ICode("EncodeTask", taskType), kamsi(nullptr) {};
public:
    static ICode* Create(Kamsi* kamsi, const std::string& url);
    virtual ~EncodeTask();
    void execute() override;
    int GetStreamInfo() override;
    const AVOutputFormat* GetOutputFormat();
    bool RequireFileHandle();
    void OpenFile();
    int Seek(int streamIndex, int64_t minTimetamp, int64_t timestamp, int64_t maxTimestamp, int tags);
};
