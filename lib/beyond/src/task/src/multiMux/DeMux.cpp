#include "DeMux.h"

/**
 * @brief  Create DeMuxTask instance.
 *
 * @return IMultiMux*  Created DeMuxTask instance pointer.
 * 
 * Dynamically allocates DeMuxTask instance,
 * Initialize the Kamsi looger,
 *
 */
IMultiMux* DeMuxTask::Create(Kamsi* kamsi) {
    DeMuxTask* decode = new DeMuxTask(TaskType::DEMUX, kamsi);
    decode->kamsi->Register();
    return decode;
}

DeMuxTask::~DeMuxTask() {
    if (kamsi) {
        kamsi->UnRegister();
        kamsi = nullptr;
    }
    
}

void DeMuxTask::execute() {
    return;
}

/**
 * 
 * @brief Read next frame of a stream.
 * 
 * @param ifc input format context.
 * @param pkt The packet to be initialized, the returned packet is reference-counted (pkt->buf is set)
 *
 * @return 0 if OK, < 0 on error or end of file. On error, pkt will be blank.
 */
int DeMuxTask::ReadFrame(AVFormatContext* ifc, AVPacket* pkt) {
    int ret = 0;
    if ((ret = av_read_frame(ifc, pkt) < 0)) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(getClassName(), "ReadFrame", std::string("Failed to demux packet: ") + errBuf);
    }
    return ret;
}
