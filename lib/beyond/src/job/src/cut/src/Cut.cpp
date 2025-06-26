
#include "Cut.h"

IJob* CutJob::Create(Kamsi* kamsi) {
    CutJob* job = new CutJob();
    job->type = JobType::CUT;
    job->tasks = Tasks::Create();
    job->kamsi = kamsi;
    job->kamsi->Register();
    return job;
}

CutJob::~CutJob() {
    delete tasks;
    tasks = nullptr;
    if (kamsi) {
        kamsi->UnRegister();
        kamsi = nullptr;
    }
}

void CutJob::SetVariables(const SOptionVariables* options, const SFlagVariables* flags) {
    TaskFactory* factory = TaskFactory::Create();

    auto createTaskFunc = factory->GetCreate(TaskType::INPUT);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get INPUT task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->inputUrl))));


    createTaskFunc = factory->GetCreate(TaskType::OUTPUT);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get OUTPUT task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(options->outputUrl))));


    createTaskFunc = factory->GetCreate(TaskType::FILEBRIDGE);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get FILEBRIDGE task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any{}));


    createTaskFunc = factory->GetCreate(TaskType::DECODE);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get DECODE task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any(getInputTask()->GetUrl())));


    createTaskFunc = factory->GetCreate(TaskType::ENCODE);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get ENCODE task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any(std::string(getOutputTask()->GetUrl()))));

    createTaskFunc = factory->GetCreate(TaskType::DEMUX);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get DEMUX task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any{}));

    createTaskFunc = factory->GetCreate(TaskType::MUX);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get MUX task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any{}));

    createTaskFunc = factory->GetCreate(TaskType::PACKET);
    if (!createTaskFunc) {
        kamsi->Error(getClassName(), "SetVariables", "Failed to get PACKET task creator");
        return;
    }
    tasks->Add(createTaskFunc(std::any{}));

    startOffset = options->startOffset;
    endOffset = options->endOffset;

    delete factory;
    return;
}

InputTask* CutJob::getInputTask() {
    return dynamic_cast<InputTask*>(tasks->Get(0));
}

OutputTask* CutJob::getOutputTask() {
    return dynamic_cast<OutputTask*>(tasks->Get(1));
}

FileBridgeTask* CutJob::getFileBridgeTask() {
    return dynamic_cast<FileBridgeTask*>(tasks->Get(2));
}

DecodeTask* CutJob::getDecodeTask() {
    return dynamic_cast<DecodeTask*>(tasks->Get(3));
}

EncodeTask* CutJob::getEncodeTask() {
    return dynamic_cast<EncodeTask*>(tasks->Get(4));
}

DeMuxTask* CutJob::getDeMuxTask() {
    return dynamic_cast<DeMuxTask*>(tasks->Get(5));
}

MuxTask* CutJob::getMuxTask() {
    return dynamic_cast<MuxTask*>(tasks->Get(6));
}

PacketTask* CutJob::getPacketTask() {
    return dynamic_cast<PacketTask*>(tasks->Get(7));
}

void CutJob::Execute() {

    const std::string& input = getInputTask()->GetUrl();
    const std::string& output = getOutputTask()->GetUrl();

    getOutputTask()->CreateDirectory();
    
    getDecodeTask()->GetStreamInfo();
    printf("stream %u codecpar = %p\n", 0, getDecodeTask()->GetFormatCtx()->streams[0]->codecpar);
    unsigned int streamCnt = getDecodeTask()->GetStreamCount();

    

    std::vector<int64_t> start(streamCnt);
    std::vector<int64_t> end(streamCnt);

    int streamIndex = 0;
    int videoIndex = -1;

    for (size_t index = 0; index < streamCnt; ++index) {
        AVCodecParameters* codecParam = getDecodeTask()->GetCodecAt(index);
        if (codecParam && codecParam->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = index;
            break;
        }
    }

    // stream index mapper between inStream & (Packet + outStream)
    StreamMemory* stream = getFileBridgeTask()->MallocArray(streamCnt);
    for (size_t index = 0; index < streamCnt; ++index) {
        AVStream* inStream = getDecodeTask()->GetStreamAt(index);
        AVCodecParameters* codecParam = getDecodeTask()->GetCodecAt(index);
        if (codecParam == nullptr) {
            continue;
        }
        start[index] = av_rescale_q (startOffset * AV_TIME_BASE, AV_TIME_BASE_Q, inStream->time_base);
        end[index] = av_rescale_q (endOffset * AV_TIME_BASE, AV_TIME_BASE_Q, inStream->time_base);        
        if (codecParam->codec_type != AVMEDIA_TYPE_VIDEO &&
            codecParam->codec_type != AVMEDIA_TYPE_AUDIO &&
            codecParam->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                stream[index] = -1;
                continue;
            }
        stream[index] = streamIndex++;
        getEncodeTask()->CreateStream(nullptr);
        getEncodeTask()->SetCodecAt(stream[index], getDecodeTask()->GetStreamAt(index)->codecpar);
    }

    if (getEncodeTask()->RequireFileHandle()) {
        getEncodeTask()->OpenFile();
    }
    
    getMuxTask()->WriteHeader(getEncodeTask()->GetFormatCtx(), nullptr);
    
    getDecodeTask()->Seek(getDecodeTask()->GetFormatCtx(), -1, INT64_MIN, start[videoIndex], INT64_MAX, 0);
    
    getPacketTask()->Alloc();
    int ret = 0;
    while (true) {
        ret = getDeMuxTask()->ReadFrame(getDecodeTask()->GetFormatCtx(), 
        getPacketTask()->GetPacket()
        );
        if (ret < 0)
            break;
        
        int currentIndex = getPacketTask()->GetPacket()->stream_index;

        int64_t pktPts = getPacketTask()->RescaleTimestamp(
            getDecodeTask()->GetStreamAt(currentIndex)->time_base, 
            getEncodeTask()->GetStreamAt(currentIndex)->time_base);

        if (pktPts > end[currentIndex]) {
            getPacketTask()->Clear();
            continue;
        }
        getPacketTask()->NormalizeTimestamp(start[currentIndex]);
        
        getPacketTask()->SetStreamIndex(stream[currentIndex]);

        getMuxTask()->WriteFrame(getEncodeTask()->formatContext, 
        getPacketTask()->GetPacket()
        );

        getPacketTask()->Clear();
    }
    getPacketTask()->Free();

    getMuxTask()->WriteTrailer(getEncodeTask()->formatContext);

    getFileBridgeTask()->FreeArray(stream);

    return;
}