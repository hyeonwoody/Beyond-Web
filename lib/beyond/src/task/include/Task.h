#pragma once
#include "Class.h"
#include "Kamsi.h"
#include <map>
#include <string>
#include <functional>
#include <any>

enum class TaskType {
    UNKNOWN,
    INPUT,
    OUTPUT,
    FILEBRIDGE,
    DECODE,
    ENCODE,
    DEMUX,
    MUX,
    PACKET,
};

class ITask : public IClass {
    protected:
    TaskType type;
    protected:
        ITask(const std::string& className, TaskType type)
            : IClass(className), type(type) {}
    public:
        virtual ~ITask() {};
        virtual void execute() = 0;
        virtual TaskType GetType() {
            return type;
        }
};

class TaskFactory
{
    private:
        Kamsi* sharedKamsi;
    private:
        typedef std::function<ITask*(std::any)> CreateCallback;
        typedef std::map<TaskType, CreateCallback> CreateTaskMap;
        CreateTaskMap createTaskMap;
    public:
        static TaskFactory* Create();
        ~TaskFactory();
        CreateCallback GetCreate(TaskType type);
    private:
        void add(const TaskType type, CreateCallback cb);
        void remove(const TaskType type);
};
