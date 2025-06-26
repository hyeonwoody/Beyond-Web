#pragma once
#include "Class.h"
#include "Argument.h"
#include "Tasks.h"
#include "Kamsi.h"
#include <stdlib.h>
#include <map>
#include <vector>
#include <functional>
#include <any>

enum class JobType {
    UNKNOWN,
    COPY,
    MOVE,
    STREAM,
    CUT,
    SYMBOLICLINK,
};

inline const char* JobTypeToString(JobType type) {
    switch(type) {
        case JobType::UNKNOWN: return "UNKNOWN";
        case JobType::COPY: return "COPY";
        case JobType::MOVE: return "MOVE";
        case JobType::STREAM: return "STREAM";
        case JobType::CUT: return "CUT";
        default: return "INVALID_JOBTYPE";
    }
}


class IJob : public IClass {
    protected:
        JobType type;
        Tasks* tasks;
    protected:
        IJob(const std::string& className)
        : IClass(className) {}
    public:
        pthread_t thread;
    public:
        virtual ~IJob() {};
        virtual void Execute() = 0;
        virtual void SetVariables(const SOptionVariables* options, const SFlagVariables* flags)  = 0;
        virtual JobType GetType() {
            return type;
        }
};

class JobFactory : public IClass {

    private:
        typedef std::function<IJob*()> CreateCallback;
        typedef std::map<JobType, CreateCallback> CreateJobMap;
        CreateJobMap createJobMap;
        Kamsi* kamsi;
    private:
        JobFactory()
            : IClass("JobFactory") {};
    public:
        static JobFactory* Create();
        ~JobFactory();
    public:
        IJob* CreateJob(CFlag* flag);
    private:
        void add(const JobType type, CreateCallback cb);
        void remove(const JobType type);
        JobType getJobType(const int index);
};