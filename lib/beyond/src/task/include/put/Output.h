#pragma once
#include "Put.h"
#include <fstream>
#include <string>

class OutputTask : public IPut {
private:
    OutputTask(TaskType taskType, std::string& url, MediaType mediaType) 
        : IPut(taskType, url, mediaType) {}
public:
    static ITask* Create(const std::string& url);
    std::ofstream* GetStream();
    void execute() override; 
};