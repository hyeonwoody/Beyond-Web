#include "Decode.h"

/**
 * @brief  Create DecodeTask instance.
 *
 * @param  url  input source (file, network URL)
 * @return ICode*  Created DecodeTask instance pointer.
 * 
 * Dynamically allocates DecodeTask instance,
 * Initialize the Kamsi looger,
 * Open the input media stream source using FFmpeg.
 *
 */
ICode* DecodeTask::Create(Kamsi* kamsi, const std::string& url) {
    DecodeTask* decode = new DecodeTask(TaskType::DECODE);
    decode->url = url;
    int ret = 0;
    if ((ret = avformat_open_input(&decode->formatContext, url.c_str(), nullptr, nullptr)) < 0) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(decode->getClassName(), "Init", "Could not open input source " + url + ".");
        delete decode;
        return nullptr;
    }
    decode->kamsi = kamsi;
    decode->kamsi->Register();
    return decode;
}

DecodeTask::~DecodeTask() {
    avformat_close_input(&formatContext);
    if (kamsi) {
        kamsi->UnRegister();
        kamsi = nullptr;
    }
    
}

/**
 * @brief Read packets of a media file to get stream information using FFmpeg SDK.
 * Since it is using FFmpeg SDK, the output in printed through standard output.
 * The logical file position is not changed by this function;
 * examined packets may be buffered for later processing.
 *
 * @todo Let the user decide somehow what information is needed so that
 *       we do not waste time getting stuff the user does not need.
 * @todo Log redirection to Kamsi.
 */
int DecodeTask::GetStreamInfo() {
    int ret = 0;
    if (!formatContext) {
        kamsi->Error(getClassName(), "GetStreamInfo", "Format context is null.");
        return -1;
    }

    if ((ret = avformat_find_stream_info(formatContext, nullptr)) < 0) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(getClassName(), "GetStreamInfo", std::string("Failed to find stream info : ") + errBuf);
    }
    av_dump_format(formatContext, 0, url.c_str(), 0);
    return ret;
}

int DecodeTask::Seek(int streamIndex, int64_t minTimetamp, int64_t timestamp, int64_t maxTimestamp, int tags) {
    int ret = 0;
    if ((ret = avformat_seek_file(formatContext, streamIndex, minTimetamp, timestamp, maxTimestamp, tags)) < 0) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
    }
    return ret;
}

void DecodeTask::execute() {
    return;
}