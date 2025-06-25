#include "Encode.h"

/**
 * @brief  Create EncodeTask instance.
 *
 * @param  url  input source (file, network URL)
 * @return ICode*  Created EncodeTask instance pointer.
 * 
 * Dynamically allocates EncodeTask instance,
 * Initialize the Kamsi looger,
 * Open the output media stream source using FFmpeg.
 *
 */
ICode* EncodeTask::Create(Kamsi* kamsi, const std::string& url) {
    EncodeTask* encode = new EncodeTask(TaskType::ENCODE);
    encode->url = url;
    int ret = 0;
    if ((ret = avformat_alloc_output_context2(&encode->formatContext, NULL, NULL, url.c_str())) < 0) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(encode->getClassName(), "Init", "Could not open input source " + url + ".");
        delete encode;
        return nullptr;
    }
    encode->kamsi = kamsi;
    encode->kamsi->Register();
    return encode;
}

EncodeTask::~EncodeTask() {
    if (formatContext && !(formatContext->flags & AVFMT_NOFILE))
        avio_closep (&formatContext->pb);
    avformat_free_context (formatContext);
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
int EncodeTask::GetStreamInfo() {
    int ret = 0;
    printf("BEFORE stream %u codecpar = %p\n", 0, GetFormatCtx()->streams[0]->codecpar);
    if (!formatContext) {
        kamsi->Error(getClassName(), "GetStreamInfo", "Format context is null.");
        return -1;
    }

    if ((ret = avformat_find_stream_info(formatContext, nullptr)) < 0) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
        kamsi->Error(getClassName(), "GetStreamInfo", std::string("Failed to find stream info : ") + errBuf);
    }
    printf("AFTER stream %u codecpar = %p\n", 0, GetFormatCtx()->streams[0]->codecpar);
    av_dump_format(formatContext, 0, url.c_str(), 1);
    return ret;
}

const AVOutputFormat* EncodeTask::GetOutputFormat() {
    return formatContext->oformat;
}

int EncodeTask::Seek(int streamIndex, int64_t minTimetamp, int64_t timestamp, int64_t maxTimestamp, int tags) {
    int ret = 0;
    if ((ret = avformat_seek_file(formatContext, streamIndex, minTimetamp, timestamp, maxTimestamp, tags)) < 0) {
        char errBuf[128];
        av_strerror(ret, errBuf, sizeof(errBuf));
    }
    return ret;
}

bool EncodeTask::RequireFileHandle() {
    return !(formatContext->flags & AVFMT_NOFILE);
}

void EncodeTask::OpenFile() {
    int ret = 0;
    if ((ret = avio_open(&formatContext->pb, url.c_str(), AVIO_FLAG_WRITE) < 0)) {
        char errBuf[512];
        av_strerror(ret, errBuf, sizeof(errBuf));
    }
}

void EncodeTask::execute() {
    return;
}