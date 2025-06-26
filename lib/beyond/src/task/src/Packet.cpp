#include "Packet.h"

ITask* PacketTask::Create(Kamsi *kamsi) {
    PacketTask* task = new PacketTask(TaskType::PACKET);
    task->kamsi = kamsi;
    task->kamsi->Register();
    return task;
}

PacketTask::~PacketTask() {
    kamsi->UnRegister();
}

void PacketTask::execute() {
    return;
}

AVPacket* PacketTask::GetPacket() {
    return packet;
}

int PacketTask::Alloc() {
    packet = av_packet_alloc();
    if (!packet) {
        kamsi->Error(getClassName(), "Alloc", "Failed to allocate Packet");
        return -1;
    }
    return 0;
}

void PacketTask::Clear() {
    av_packet_unref(packet);
}

void PacketTask::Free() {
    av_packet_free(&packet);
}
