#pragma once
#include "Code.h"
#include "Kamsi.h"

class DecodeTask : public ICode {
private:
    DecodeTask(TaskType taskType) 
    : ICode("DecodeTask", taskType) {};
public:
    static ICode* Create(Kamsi* kamsi, const std::string& url);
    virtual ~DecodeTask();
    void execute() override;
    int GetStreamInfo() override;
   
};
