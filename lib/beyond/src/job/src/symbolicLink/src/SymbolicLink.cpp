
#include "SymbolicLink.h"



IJob* SymbolicLinkJob::Create(Kamsi* kamsi) {
    SymbolicLinkJob* job = new SymbolicLinkJob();
    job->type = JobType::SYMBOLICLINK;
    job->tasks = Tasks::Create();
    job->kamsi = kamsi;
    return job;
}

SymbolicLinkJob::~SymbolicLinkJob() {
    delete tasks;
    delete kamsi;
}

void SymbolicLinkJob::SetVariables(const SOptionVariables* options, const SFlagVariables* flags) {
    TaskFactory* factory = TaskFactory::Create();

    auto createTaskFunc = factory->GetCreate(TaskType::INPUT);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get INPUT task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->inputUrl))));

    createTaskFunc = factory->GetCreate(TaskType::OUTPUT);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get OUTPUT task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->outputUrl))));

    createTaskFunc = factory->GetCreate(TaskType::FILEBRIDGE);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get FILEBRIDGE task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any{}));

    delete factory;
    return;
}

InputTask* SymbolicLinkJob::getInputTask() {
    return dynamic_cast<InputTask*>(tasks->Get(0));
}

OutputTask* SymbolicLinkJob::getOutputTask() {
    return dynamic_cast<OutputTask*>(tasks->Get(1));
}

FileBridgeTask* SymbolicLinkJob::getFileBridgeTask() {
    return dynamic_cast<FileBridgeTask*>(tasks->Get(2));
}

void SymbolicLinkJob::Execute() {

    const std::string& input = getInputTask()->GetUrl();
    const std::string& output = getOutputTask()->GetUrl();
   
    getOutputTask()->CreateDirectory();

    getFileBridgeTask()->Symlink(input, output);
    char buf[33 + input.length() + output.length()];
    std::snprintf(buf, sizeof(buf), "Created symbolic link from %s to %s", input, output);
    kamsi->Info(getClassName(),"CreateJob", buf);
}


