#include "Task.h"
#include "Input.h"
#include "Output.h"
#include "FileBridge.h"

TaskFactory* TaskFactory::Create() {
    TaskFactory* factory = new TaskFactory();
    factory->registerTask(TaskType::INPUT, [](std::any arg) -> ITask* {
        return InputTask::Create(std::any_cast<std::string>(arg));
    });

    factory->registerTask(TaskType::OUTPUT, [](std::any arg) -> ITask* {
        return OutputTask::Create(std::any_cast<std::string>(arg));
    });
    factory->registerTask(TaskType::FILEBRIDGE, [](std::any) -> ITask* {
        return FileBridgeTask::Create();
    });
    return factory;
}

TaskFactory::~TaskFactory()
{
    this->unRegisterTask(TaskType::INPUT);
    this->unRegisterTask(TaskType::OUTPUT);
    this->unRegisterTask(TaskType::FILEBRIDGE);
}

void TaskFactory::registerTask(TaskType type, CreateCallback cb) {
    createTaskMap[type] = cb;
}

void TaskFactory::unRegisterTask(TaskType type) {
    createTaskMap.erase(type);
}

TaskFactory::CreateCallback TaskFactory::GetCreate(TaskType type) {
    auto it = createTaskMap.find(type);
	if (it != createTaskMap.end()){
		return (it->second);
	}
	return nullptr;
}