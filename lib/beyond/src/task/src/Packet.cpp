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

size_t PacketTask::GetStreamIndex() {
    return packet->stream_index;
}

/**
 * @brief Updates the packet's stream index to match the output stream layout.
 *
 * When remuxing or transcoding, the stream index from the input (`packet->stream_index`)
 * may no longer match the output file's stream layout. For example, input stream 0 (video)
 * might be written as output stream 1 depending on stream filtering, reordering,
 * or selective copying (e.g., video-only).
 *
 * This function explicitly sets the packet's `stream_index` to the correct
 * output stream index before writing the packet via `av_interleaved_write_frame()`.
 *
 * If this step is skipped, packets may be written to the wrong stream, or
 * muxing may fail due to missing stream mappings.
  * If this step is skipped, packets may be written to the wrong stream, or
 * muxing may fail due to missing stream mappings.
 *
 * Typical usage:
 * - Map input stream indices to output stream indices (e.g., `streamMap[inputIndex] = outputIndex`)
 * - Before writing each packet, call:
 *     `pkt->SetStreamIndex(stream[pkt->stream_index]);`
 *
 * @param index The output stream index to assign to this packet.
 */
void PacketTask::SetStreamIndex(size_t index) {
    packet->stream_index = index;
}

int PacketTask::Alloc() {
    packet = av_packet_alloc();
    if (!packet) {
        kamsi->Error(getClassName(), "Alloc", "Failed to allocate Packet");
        return -1;
    }
    return 0;
}
/**
 * @brief Converts the current packet's PTS from one stream's time base to another using FFmpeg's av_rescale_q.
 * 
 * This is commonly used when remuxing or transcoding, where timestamps need to be adjusted
 * between different stream time bases to maintain synchronization.
 * 
 * @param inStreamTimeBase  The time base of the input stream.
 * @param outStreamTimeBase The time base of the output stream.
 * 
 * @return The rescaled timestamp.
 */
int64_t PacketTask::RescaleTimestamp(AVRational inStreamTimeBase, AVRational outStreamTimeBase) {
    if (packet->pts != AV_NOPTS_VALUE)
        packet->pts = av_rescale_q(packet->pts, inStreamTimeBase, outStreamTimeBase);
    if (packet->dts != AV_NOPTS_VALUE)
        packet->dts = av_rescale_q(packet->dts, inStreamTimeBase, outStreamTimeBase);
    if (packet->duration > 0)
        packet->duration = av_rescale_q(packet->duration, inStreamTimeBase, outStreamTimeBase);
    return packet->pts;
}

/**
 * @brief Re-bases the packet’s PTS/DTS so that a chosen timestamp becomes 0.
 *
 * For a clean clip that starts at 0 s, we subtract that first-seen value from
 * every subsequent packet.  This “normalizes” the timestamps, letting players
 * and muxers treat the new segment as an independent file that begins at
 * 0 s and lasts exactly its displayed duration.
 *
 *
 * @param relativeZero  The baseline timestamp to subtract (usually the first
 *                      PTS/DTS encountered after a successful seek), expressed
 *                      in the packet’s current time-base units.
 */
void PacketTask::NormalizeTimestamp(int64_t relativeZero) {
    packet->pts -= relativeZero;
    packet->dts -= relativeZero;
}

void PacketTask::Clear() {
    av_packet_unref(packet);
}

void PacketTask::Free() {
    av_packet_free(&packet);
}
