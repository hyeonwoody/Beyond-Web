#pragma once
#include "Code.h"
#include "Kamsi.h"

class EncodeTask : public ICode {
private:
    EncodeTask(TaskType taskType) 
    : ICode("EncodeTask", taskType) {};
public:
    static ICode* Create(Kamsi* kamsi, const std::string& url);
    virtual ~EncodeTask();
    void execute() override;
    int GetStreamInfo() override;
    const AVOutputFormat* GetOutputFormat();
    bool RequireFileHandle();
    void OpenFile();
};
