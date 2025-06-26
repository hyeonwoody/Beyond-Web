#pragma once
#include "Task.h"
extern "C" {

    #include <libavcodec/packet.h>
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
        AVPacket* GetPacket();
        int Alloc();
        void Clear();
        void Free();
};