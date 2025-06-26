#pragma once
#include "Task.h"
extern "C" {
    #include "libavcodec/packet.h"
    #include "libavutil/avutil.h"
    #include "libavutil/mathematics.h"
}
class PacketTask : public ITask {
    private:
        Kamsi* kamsi;
    private:
        AVPacket* packet;
    private:
        PacketTask(TaskType taskType)
        : ITask("PacketTask", taskType){}
    public:
        static ITask* Create(Kamsi* kamsi);
        ~PacketTask();
        void execute() override;
        size_t GetStreamIndex();
        void SetStreamIndex(size_t index);
        AVPacket* GetPacket();
        int Alloc();
        void Clear();
        void Free();
        int64_t RescaleTimestamp(AVRational inStreamTimeBase, AVRational outStreamTimeBase);
        void NormalizeTimestamp(int64_t relativeZero);
};