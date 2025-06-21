#include "Tasks.h"
#include "Argument.h"


Tasks* Tasks::Create() {
    return new Tasks();
}

Tasks::~Tasks() {
    for (ITask* task : tasks) {
        delete task;
    }
    tasks.clear();
}

void Tasks::Add(ITask* task) {
    tasks.push_back(task);
}

ITask* Tasks::Get(int index) {
    if (index < 0 || index >= static_cast<int>(tasks.size())) {
        return nullptr;
    }
    return tasks[index];
}
