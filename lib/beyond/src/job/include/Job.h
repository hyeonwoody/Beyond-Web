#pragma once
#include "Argument.h"
#include "Tasks.h"
#include <stdlib.h>
#include <map>
#include <vector>

enum class JobType {
    UNKNOWN,
    COPY,
    MOVE,
    STREAM,
    CUT,
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


class IJob {
    protected:
        JobType type;
        Tasks* tasks;
    public:
        virtual void Execute() = 0;
        virtual void SetVariables(const SOptionVariables* options, const SFlagVariables* flags)  = 0;
        virtual JobType GetType() {
            return type;
        }
};

class JobFactory
{

    private:
        typedef IJob* (*CreateCallback)();
        typedef std::map<JobType, CreateCallback> CreateJobMap;
        CreateJobMap createJobMap;
    public:
        static JobFactory* Create();
        ~JobFactory();
    public:
        IJob* CreateJob(CFlag* flag);
    private:
        void registerJob(const JobType type, CreateCallback cb);
        void unRegisterJob(const JobType type);
        JobType getJobType(const int index);
};