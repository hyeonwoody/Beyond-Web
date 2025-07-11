#include "Output.h"


ITask* OutputTask::Create(Kamsi* kamsi, const std::string& output){
    auto [type, parsedUrl] = parseMediaType(output);
    OutputTask* task = new OutputTask(TaskType::OUTPUT, parsedUrl, type);
    task->kamsi = kamsi;
    task->kamsi->Register();
    return task;
};

OutputTask::~OutputTask() {
    kamsi->UnRegister();
}

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
            kamsi->Error(getClassName(), "createDirectory", "Failed to create output directory");
            kamsi->Error(getClassName(), "createDirectory", "Reason : " + ec.message());
            return;
        }
    }
}