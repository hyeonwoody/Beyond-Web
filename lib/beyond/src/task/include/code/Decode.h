#pragma once
#include "Code.h"
#include "Kamsi.h"

class DecodeTask : public ICode {
private:
    Kamsi* kamsi;
private:
    DecodeTask(TaskType taskType) 
    : ICode("DecodeTask", taskType), kamsi(nullptr) {};
public:
    static ICode* Create(Kamsi* kamsi, const std::string& url);
    virtual ~DecodeTask();
    void execute() override;
    int GetStreamInfo() override;
    int Seek(int streamIndex, int64_t minTimetamp, int64_t timestamp, int64_t maxTimestamp, int tags);
};
