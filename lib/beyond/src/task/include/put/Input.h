#pragma once
#include "Put.h"
#include <fstream>
#include <string>

class InputTask : public IPut {
private:
    private:
    InputTask(TaskType taskType, const std::string& url, MediaType mediaType) 
            : IPut(taskType, url, mediaType) {}

public:
    static ITask* Create(const std::string& input);
    std::ifstream* GetStream();
    void execute() override; 
};