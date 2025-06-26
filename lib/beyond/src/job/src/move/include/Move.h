#pragma once
#include "Job.h"
#include "Copy.h"
#include "Input.h"
#include "string"

class MoveJob : public IJob {
    private:
        IJob* copyJob;
    private:
        MoveJob() 
            :IJob("MoveJob") {};
    public:
        static IJob* Create();
        ~MoveJob();
        void Execute() override;
        
        void SetVariables(const SOptionVariables* options, const SFlagVariables* flags) override;
    private:
        InputTask* getInputTask();
    private:
        void removeInput();
};
