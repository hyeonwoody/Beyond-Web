#include "Task.h"
#include "Input.h"
#include "Output.h"
#include "FileBridge.h"
#include "Decode.h"

TaskFactory* TaskFactory::Create() {
    TaskFactory* factory = new TaskFactory();
    Kamsi* kamsi = Kamsi::Create("Init");
    factory->add(TaskType::INPUT, [](std::any arg) -> ITask* {
        return InputTask::Create(std::any_cast<std::string>(arg));
    });

    factory->add(TaskType::OUTPUT, [kamsi](std::any arg) -> ITask* {
        return OutputTask::Create(kamsi, std::any_cast<std::string>(arg));
    });
    factory->add(TaskType::FILEBRIDGE, [](std::any) -> ITask* {
        return FileBridgeTask::Create();
    });
    return factory;
}

TaskFactory::~TaskFactory()
{
    this->remove(TaskType::INPUT);
    this->remove(TaskType::OUTPUT);
    this->remove(TaskType::FILEBRIDGE);
}

void TaskFactory::add(const TaskType type, CreateCallback cb) {
    createTaskMap[type] = cb;
}

void TaskFactory::remove(TaskType type) {
    createTaskMap.erase(type);
}

TaskFactory::CreateCallback TaskFactory::GetCreate(TaskType type) {
    auto it = createTaskMap.find(type);
	if (it != createTaskMap.end()){
		return (it->second);
	}
	return nullptr;
}