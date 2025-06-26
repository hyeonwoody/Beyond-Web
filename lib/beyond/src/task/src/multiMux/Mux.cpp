#include "Mux.h"

/**
 * @brief  Create MuxTask instance.
 *
 * @return IMultiMux*  Created MuxTask instance pointer.
 * 
 * Dynamically allocates MuxTask instance,
 * Initialize the Kamsi looger,
 *
 */
IMultiMux* MuxTask::Create(Kamsi* kamsi) {
    MuxTask* decode = new MuxTask(TaskType::MUX, kamsi);
    decode->kamsi->Register();
    return decode;
}

MuxTask::~MuxTask() {
    if (kamsi) {
        kamsi->UnRegister();
        kamsi = nullptr;
    }
    
}

void MuxTask::execute() {
    return;
}

/**
 * 
 * @brief Write the stream private data and write the stream header.
 * 
 * @param ofc output format context, media file handler.
 * @param opt An AVDictionary filled with AVFormatContext and muxer-private options. May be NULL.
 */
void MuxTask::WriteHeader(AVFormatContext* ofc, AVDictionary** opt) {
    int ret = 0;
    if ((ret =avformat_write_header(ofc, opt)) < 0) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(getClassName(), "WriteHeader", std::string("Failed to open output file: ") + errBuf);
    }
}

/**
 * 
 * @brief Write a packet to an output file.
 * 
 * @param ofc output format context, media file handler.
 * @param pkt The packet containing the data to be written.
 *
 * 
 * av_interleaved_write_frame() will buffer the packets internally as needed to make sure the
 * packets in the output file are properly interleaved in the order of
 * increasing dts. Callers doing their own interleaving should call
 * av_write_frame() instead of this function.
 *
 *
 * @see av_write_frame(), AVFormatContext.max_interleave_delta
 */
void MuxTask::WriteFrame(AVFormatContext* ofc, AVPacket* pkt) {
    int ret = 0;
    if ((ret = av_interleaved_write_frame(ofc, pkt) < 0)) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(getClassName(), "WriteFrame", std::string("Failed to mux packet: ") + errBuf);
    }
}

/**
 * Write the stream trailer to an output media file and free the
 * file private data.
 *
 * Trailer contains seek tables, duration,and metadata.
 *
 * @param s media file handle
 * @return 0 if OK, AVERROR_xxx on error
 */
void MuxTask::WriteTrailer(AVFormatContext* ofc) {
    int ret = 0;
    if ((ret = av_write_trailer(ofc) < 0)) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(getClassName(), "WriteTrailer", std::string("Failed to finalize: ") + errBuf);
    }
}