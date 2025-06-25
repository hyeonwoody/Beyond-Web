#pragma once
#include "Task.h"
class IBridge : public ITask {
    protected:
        IBridge(const std::string& className, TaskType taskType)
        : ITask(className, taskType){}
    public:
        virtual ~IBridge() = default;
        
};