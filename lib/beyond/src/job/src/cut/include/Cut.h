#pragma once
#include "Job.h"
#include "Input.h"
#include "Output.h"
#include "Decode.h"
#include "Encode.h"
#include "DeMux.h"
#include "Mux.h"
#include "FileBridge.h"
#include "Packet.h"
#include "Kamsi.h"
#include <string>

class CutJob : public IJob {
    private:
        Kamsi* kamsi;
        int64_t startOffset;
        int64_t endOffset;
    private:
        CutJob()
        : IJob("CutJob") {};
    public:
        static IJob* Create(Kamsi* kamsi);
        ~CutJob();
        void Execute() override;
        
        void SetVariables(const SOptionVariables* options, const SFlagVariables* flags) override;
    private:
        InputTask* getInputTask();
        OutputTask* getOutputTask();
        DecodeTask* getDecodeTask();
        EncodeTask* getEncodeTask();
        FileBridgeTask* getFileBridgeTask();
        DeMuxTask* getDeMuxTask();
        MuxTask* getMuxTask();
        PacketTask* getPacketTask();
};
