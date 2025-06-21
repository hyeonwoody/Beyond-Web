#include "Input.h"


ITask* InputTask::Create(const std::string& input){
    auto [type, parsedUrl] = parseMediaType(input);
    return new InputTask(TaskType::INPUT, parsedUrl, type);
};

void InputTask:: execute() {
    
};

std::ifstream* InputTask::GetStream() {
    return new std::ifstream(this->GetUrl(), std::ios::binary);
}