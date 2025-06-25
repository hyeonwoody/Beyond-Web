#pragma once
#include "Job.h"
#include "Input.h"
#include "Output.h"
#include "FileBridge.h"
#include "Kamsi.h"
#include <string>

class SymbolicLinkJob : public IJob {
    private:
        Kamsi* kamsi;
    private:
        SymbolicLinkJob()
            :IJob("SymbolicLinkJob") {}
    public:
        static IJob* Create();
        ~SymbolicLinkJob();
        void Execute() override;
        
        void SetVariables(const SOptionVariables* options, const SFlagVariables* flags) override;
    private:
        InputTask* getInputTask();
        OutputTask* getOutputTask();
        FileBridgeTask* getFileBridgeTask();
    private:
        void createDirectory(const std::string& path);
};
