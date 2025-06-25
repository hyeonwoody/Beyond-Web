#pragma once
#include "Put.h"
#include "Kamsi.h"
#include <fstream>
#include <string>

class OutputTask : public IPut {
private:
    Kamsi* kamsi;
private:
    OutputTask(TaskType taskType, std::string& url, MediaType mediaType) 
        : IPut("OutputTask",taskType, url, mediaType) {}
public:
    ~OutputTask();
    static ITask* Create(Kamsi* kamsi, const std::string& url);
    std::ofstream* GetStream();
    void execute() override; 
    void CreateDirectory();
};