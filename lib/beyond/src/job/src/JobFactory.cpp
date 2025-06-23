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

#ifdef ENABLE_COPY
    factory->add(JobType::COPY, &CopyJob::Create);
#endif

#ifdef ENABLE_MOVE
    factory->add(JobType::MOVE, &MoveJob::Create);
#endif
#ifdef ENABLE_SYMBOLICLINK
    factory->add(JobType::SYMBOLICLINK, &SymbolicLinkJob::Create);
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
    std::cout << "📢[CreateJob] Type " << JobTypeToString(type) << " is not available in this application." << std::endl;
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