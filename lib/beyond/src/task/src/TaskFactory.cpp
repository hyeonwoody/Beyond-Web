#include "Task.h"
#include "Input.h"
#include "Output.h"
#include "FileBridge.h"
#include "Decode.h"
#include "Encode.h"

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
    factory->add(TaskType::DECODE, [kamsi](std::any arg) -> ITask* {
        return DecodeTask::Create(kamsi, std::any_cast<std::string>(arg));
    });
    factory->add(TaskType::ENCODE, [kamsi](std::any arg) -> ITask* {
        return EncodeTask::Create(kamsi, std::any_cast<std::string>(arg));
    });
    return factory;
}

TaskFactory::~TaskFactory()
{
    this->remove(TaskType::INPUT);
    this->remove(TaskType::OUTPUT);
    this->remove(TaskType::FILEBRIDGE);
    this->remove(TaskType::DECODE);
    this->remove(TaskType::ENCODE);
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