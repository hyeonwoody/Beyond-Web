#pragma once
#include "Bridge.h"
#include <fstream>

class FileBridgeTask : public IBridge {
    private:
    FileBridgeTask(TaskType taskType) 
    : IBridge(taskType) {}
    public:
        static FileBridgeTask* Create();
        void Transfer(std::ifstream& ifstream, std::ofstream& ofstream);
        void execute() override;
};
