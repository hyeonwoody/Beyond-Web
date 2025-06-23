
#include "SymbolicLink.h"



IJob* SymbolicLinkJob::Create() {
    SymbolicLinkJob* job = new SymbolicLinkJob();
    job->type = JobType::SYMBOLICLINK;
    job->tasks = Tasks::Create();
    return job;
}

SymbolicLinkJob::~SymbolicLinkJob() {
    delete tasks;
}

void SymbolicLinkJob::SetVariables(const SOptionVariables* options, const SFlagVariables* flags) {
    TaskFactory* factory = TaskFactory::Create();

    auto createTaskFunc = factory->GetCreate(TaskType::INPUT);
    if (!createTaskFunc) {
        std::cerr << "⛔[SymbolicLinkJob::SetVariables] Failed to get INPUT task creator" << std::endl;
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->inputUrl))));

    createTaskFunc = factory->GetCreate(TaskType::OUTPUT);
    if (!createTaskFunc) {
        std::cerr << "⛔[SymbolicLinkJob::SetVariables] Failed to get INPUT task creator" << std::endl;
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->outputUrl))));

    createTaskFunc = factory->GetCreate(TaskType::FILEBRIDGE);
    if (!createTaskFunc) {
        std::cerr << "⛔[SymbolicLinkJob::SetVariables] Failed to get FILEBRIDGE task creator" << std::endl;
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

    createDirectory(getOutputTask()->GetPath());

    getFileBridgeTask()->Symlink(input, output);
    std::cout << "🔊[SymbolicLinkJob::Execute] Created symbolic link from " << input << " to " << output << "\n";
}

void SymbolicLinkJob::createDirectory(const std::string& path) {
    if (!path.empty() && !fs::exists(path)) {
        std::error_code ec;
        if (!fs::create_directories(path, ec)) {
            std::cerr << "⛔[SymbolicLinkJob::createDirectory] Failed to create output directory: " << path << "\n";
            std::cerr << "Reason: " << ec.message() << "\n";
            return;
        }
    }
}
