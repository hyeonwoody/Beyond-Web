#pragma once
#include "vector"
#include "Task.h"

class Tasks {
    private:
        std::vector<ITask*> tasks;
    public:
        ~Tasks();
        static Tasks* Create();
        void Add(ITask* task);
        ITask* Get(int index);
};