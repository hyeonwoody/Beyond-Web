#include "Output.h"


ITask* OutputTask::Create(const std::string& output){
    auto [type, parsedUrl] = parseMediaType(output);
    OutputTask* task = new OutputTask(TaskType::OUTPUT, parsedUrl, type);
    task->kamsi = Kamsi::Create("OutputTask", "Init");
    return task;
};


void OutputTask::execute() {
    
};

std::ofstream* OutputTask::GetStream() {
    return new std::ofstream(this->GetUrl(), std::ios::binary);
}

void OutputTask::CreateDirectory() {
    const std::string& path = GetPath();
    if (!url.empty() && !fs::exists(path)) {
        std::error_code ec;
        if (!fs::create_directories(path, ec)) {
            kamsi->Error("createDirectory", "Failed to create output directory");
            kamsi->Error("createDirectory", "Reason : " + ec.message());
            return;
        }
    }
}