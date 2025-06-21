#pragma once
#include <map>
#include <string>
#include <functional>
#include <any>

enum class TaskType {
    UNKNOWN,
    INPUT,
    OUTPUT,
    FILEBRIDGE,
};

class ITask {
    protected:
        TaskType type;
        ITask(TaskType type) : type(type) {}

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
        typedef std::function<ITask*(std::any)> CreateCallback;
        typedef std::map<TaskType, CreateCallback> CreateTaskMap;
        CreateTaskMap createTaskMap;
    public:
        static TaskFactory* Create();
        ~TaskFactory();
        CreateCallback GetCreate(TaskType type);
    private:
        void registerTask(const TaskType type, CreateCallback cb);
        void unRegisterTask(const TaskType type);
};
