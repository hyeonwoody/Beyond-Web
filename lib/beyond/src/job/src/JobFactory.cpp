#include "Job.h"

#ifdef ENABLE_COPY
#include "Copy.h"
#endif

#ifdef ENABLE_MOVE
#include "Move.h"
#endif

JobFactory* JobFactory::Create() {
    JobFactory* factory = new JobFactory();

#ifdef ENABLE_COPY
    factory->registerJob(JobType::COPY, &CopyJob::Create);
#endif

#ifdef ENABLE_MOVE
    factory->registerJob(JobType::MOVE, &MoveJob::Create);
#endif

    return factory;
}

JobFactory::~JobFactory(){
#ifdef ENABLE_COPY
    this->unRegisterJob(JobType::COPY);
#endif
#ifdef ENABLE_MOVE
    this->unRegisterJob(JobType::MOVE);
#endif
}

void JobFactory::registerJob(JobType type, CreateCallback cb) {
    createJobMap[type] = cb;
}

void JobFactory::unRegisterJob(JobType type) {
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
    std::cout << "📢[CreateJob] Type " << JobTypeToString(type) << " is not available in this application." << std::endl;
	return nullptr;
}

JobType JobFactory::getJobType(int index) {
    switch (index) {
        case 0: return JobType::COPY;
        case 1: return JobType::MOVE;
        case 2: return JobType::STREAM;
        case 3: return JobType::CUT;
        default: return JobType::UNKNOWN;
    }
}