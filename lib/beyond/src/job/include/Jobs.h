#pragma once
#include "Job.h"
#include "Kamsi.h"
#include <vector>
#include <pthread.h>
#include <mutex>


class Jobs : public IClass {
    private:
        std::vector<IJob*> jobs;
        std::mutex jobMutex;

        Kamsi* kamsi;
    private:
        Jobs()
        : IClass("Jobs") {}

    private:
        struct ExecutorParam {
            Jobs* jobs;
            IJob* job;
        };
    public:
        static Jobs* Create();
        ~Jobs();
        void Add(IJob* job);
        void Remove(IJob* job);
        void Sort();
        int Start();
        static void* jobExecutor(void* arg);
        static void* jobCleanUp(IJob* job);
}; 


