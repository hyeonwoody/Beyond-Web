#include "FileBridge.h"

FileBridgeTask* FileBridgeTask::Create(){
    return new FileBridgeTask(TaskType::FILEBRIDGE);
};

void FileBridgeTask::Transfer(std::ifstream& ifstream, std::ofstream& ofstream) {
    ofstream << ifstream.rdbuf();
}

void FileBridgeTask::execute() {
    return;
}