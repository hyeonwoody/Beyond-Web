#pragma once
#include "vector"
#include "Job.h"

class Jobs {
        std::vector<IJob*> jobs;
    public:
        static Jobs* Create();
        void Add(IJob* job);
        void Sort();
        int Start();
};


