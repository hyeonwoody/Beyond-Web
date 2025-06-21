#include "Output.h"


ITask* OutputTask::Create(const std::string& output){
    auto [type, parsedUrl] = parseMediaType(output);
    return new OutputTask(TaskType::OUTPUT, parsedUrl, type);
};


void OutputTask::execute() {
    
};

std::ofstream* OutputTask::GetStream() {
    return new std::ofstream(this->GetUrl(), std::ios::binary);
}