#include "Task.h"
#include "Input.h"
#include "Output.h"
#include "FileBridge.h"
#include "Decode.h"
#include "Encode.h"
#include "DeMux.h"
#include "Mux.h"

TaskFactory* TaskFactory::Create() {
    TaskFactory* factory = new TaskFactory();
    Kamsi* rawKamsi = Kamsi::Create("Init");
    rawKamsi->Register();
    factory->sharedKamsi = rawKamsi;
    Kamsi* sharedKamsi = rawKamsi; // local variable for lamda capture.
    factory->add(TaskType::INPUT, [](std::any arg) -> ITask* {
        return InputTask::Create(std::any_cast<std::string>(arg));
    });

    factory->add(TaskType::OUTPUT, [sharedKamsi](std::any arg) -> ITask* {
        return OutputTask::Create(sharedKamsi, std::any_cast<std::string>(arg));
    });
    factory->add(TaskType::FILEBRIDGE, [](std::any) -> ITask* {
        return FileBridgeTask::Create();
    });
    factory->add(TaskType::DECODE, [sharedKamsi](std::any arg) -> ITask* {
        return DecodeTask::Create(sharedKamsi, std::any_cast<std::string>(arg));
    });
    factory->add(TaskType::ENCODE, [sharedKamsi](std::any arg) -> ITask* {
        return EncodeTask::Create(sharedKamsi, std::any_cast<std::string>(arg));
    });
    factory->add(TaskType::MUX, [sharedKamsi](std::any arg) -> ITask* {
        return MuxTask::Create(sharedKamsi);
    });
    factory->add(TaskType::DEMUX, [sharedKamsi](std::any arg) -> ITask* {
        return DeMuxTask::Create(sharedKamsi);
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
    this->remove(TaskType::DEMUX);
    this->remove(TaskType::MUX);
    if (sharedKamsi) {
        sharedKamsi->UnRegister();
        sharedKamsi = nullptr;
    }
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