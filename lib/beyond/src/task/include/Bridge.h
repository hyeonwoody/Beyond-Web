#pragma once
#include "Task.h"
class IBridge : public ITask {
    protected:
        IBridge(TaskType taskType)
        : ITask(taskType){}
    public:
        virtual ~IBridge() = default;
        
};