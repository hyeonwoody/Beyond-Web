#include "Job.h"

#ifdef ENABLE_COPY
#include "Copy.h"
#endif

#ifdef ENABLE_MOVE
#include "Move.h"
#endif

#ifdef ENABLE_SYMBOLICLINK
#include "SymbolicLink.h"
#endif

JobFactory* JobFactory::Create() {
    JobFactory* factory = new JobFactory();
    factory->kamsi = Kamsi::Create("Init");
    factory->kamsi->Register();
    Kamsi* jobKamsi = Kamsi::Create("Init");

#ifdef ENABLE_COPY
    factory->add(JobType::COPY, [jobKamsi]() -> IJob* {
        return CopyJob::Create(jobKamsi);
    });
#endif
#ifdef ENABLE_MOVE
    factory->add(JobType::MOVE, []() -> IJob* {
        return MoveJob::Create();
    });
#endif
#ifdef ENABLE_SYMBOLICLINK
    factory->add(JobType::SYMBOLICLINK, [jobKamsi]() -> IJob* {
        return SymbolicLinkJob::Create(jobKamsi);
    });
#endif
    return factory;
}

JobFactory::~JobFactory(){
#ifdef ENABLE_COPY
    this->remove(JobType::COPY);
#endif
#ifdef ENABLE_MOVE
    this->remove(JobType::MOVE);
#endif
#ifdef ENABLE_SYMBOLICLINK
    this->remove(JobType::SYMBOLICLINK);
#endif
    
    if (kamsi) {
        kamsi->UnRegister();
        kamsi = nullptr;
    }
}

void JobFactory::add(JobType type, CreateCallback cb) {
    createJobMap[type] = cb;
}

void JobFactory::remove(JobType type) {
    createJobMap.erase(type);
}

IJob* JobFactory::CreateJob(CFlag* flag) {
    JobType type = getJobType(flag->index);
    if (type == JobType::UNKNOWN) {
        return nullptr;
    }
    
    auto it = createJobMap.find(type);
	if (it != createJobMap.end()){
		return (it->second());
	}
    char buf[64];
    std::snprintf(buf, sizeof(buf), "Type %s is not available in this application.", JobTypeToString(type));
    kamsi->Info(getClassName(), "CreateJob", buf);
	return nullptr;
}

JobType JobFactory::getJobType(int index) {
    switch (index) {
        case 0: return JobType::COPY;
        case 1: return JobType::MOVE;
        case 2: return JobType::STREAM;
        case 3: return JobType::CUT;
        case 4: return JobType::SYMBOLICLINK;
        default: return JobType::UNKNOWN;
    }
}