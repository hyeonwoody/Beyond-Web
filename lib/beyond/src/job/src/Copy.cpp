
#include "Copy.h"



IJob* CopyJob::Create() {
    CopyJob* job = new CopyJob();
    job->type = JobType::COPY;
    job->tasks = Tasks::Create();
    return job;
}

CopyJob::~CopyJob() {
    delete tasks;
}

void CopyJob::SetVariables(const SOptionVariables* options, const SFlagVariables* flags) {
    TaskFactory* factory = TaskFactory::Create();

    auto createTaskFunc = factory->GetCreate(TaskType::INPUT);
    if (!createTaskFunc) {
        std::cerr << "Failed to get INPUT task creator" << std::endl;
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->inputUrl))));

    createTaskFunc = factory->GetCreate(TaskType::OUTPUT);
    if (!createTaskFunc) {
        std::cerr << "Failed to get INPUT task creator" << std::endl;
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->outputUrl))));

    createTaskFunc = factory->GetCreate(TaskType::FILEBRIDGE);
    if (!createTaskFunc) {
        std::cerr << "Failed to get FILEBRIDGE task creator" << std::endl;
        return;
    }
    tasks->Add(createTaskFunc(std::any{}));

    delete factory;
    return;
}

InputTask* CopyJob::getInputTask() {
    return dynamic_cast<InputTask*>(tasks->Get(0));
}

OutputTask* CopyJob::getOutputTask() {
    return dynamic_cast<OutputTask*>(tasks->Get(1));
}

FileBridgeTask* CopyJob::getFileBridgeTask() {
    return dynamic_cast<FileBridgeTask*>(tasks->Get(2));
}

void CopyJob::Execute() {

    const std::string& input = getInputTask()->GetUrl();
    const std::string& output = getOutputTask()->GetUrl();

    auto ifstream = getInputTask()->GetStream();
    if (!ifstream || !ifstream->is_open()) {
        std::cerr << "[CopyJob] Cannot open input file: " << input << "\n";
        delete ifstream;
        return;
    }
    auto ofstream = getOutputTask()->GetStream();
    if (!ofstream || !ofstream->is_open()) {
        std::cerr << "[CopyJob] Cannot open output file: " << output << "\n";
        delete ifstream;
        delete ofstream;
        return;
    }
    createDirectory(getOutputTask()->GetPath());
    getFileBridgeTask()->Transfer(*ifstream, *ofstream);
    std::cout << "[CopyJob] Copied from " << input << " to " << output << "\n";
    delete ifstream;
    delete ofstream;
}

void CopyJob::createDirectory(const std::string& path) {
    if (!path.empty() && !fs::exists(path)) {
        std::error_code ec;
        if (!fs::create_directories(path, ec)) {
            std::cerr << "[CopyJob] Failed to create output directory: " << path << "\n";
            std::cerr << "Reason: " << ec.message() << "\n";
            return;
        }
    }
}
