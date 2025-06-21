
#include "Move.h"
#include "Input.h"

IJob* MoveJob::Create() {
    MoveJob* job = new MoveJob();
    job->type = JobType::MOVE;
    job->tasks = Tasks::Create();
    return job;
}

MoveJob::~MoveJob() {
    delete tasks;
}

void MoveJob::SetVariables(const SOptionVariables* options, const SFlagVariables* flags) {
    TaskFactory* factory = TaskFactory::Create();
    copyJob->SetVariables(options, flags);
    auto createTaskFunc = factory->GetCreate(TaskType::INPUT);
    if (!createTaskFunc) {
        std::cerr << "Failed to get INPUT task creator" << std::endl;
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->inputUrl))));
    delete factory;
    return;
}

InputTask* MoveJob::getInputTask() {
    return dynamic_cast<InputTask*>(tasks->Get(0));
}

void MoveJob::Execute() {
    copyJob->Execute();
    getInputTask()->Remove();
}

void MoveJob::removeInput() {
    
}

