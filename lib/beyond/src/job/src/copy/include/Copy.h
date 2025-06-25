#pragma once
#include "Job.h"
#include "Input.h"
#include "Output.h"
#include "FileBridge.h"
#include "string"

class CopyJob : public IJob {
    private:
        Kamsi* kamsi;
    private:
        CopyJob()
            : IJob("CopyJob") {};
    public:
        static IJob* Create(Kamsi* kamsi);
        ~CopyJob();
        void Execute() override;
        
        void SetVariables(const SOptionVariables* options, const SFlagVariables* flags) override;
    private:
        InputTask* getInputTask();
        OutputTask* getOutputTask();
        FileBridgeTask* getFileBridgeTask();

        
};
