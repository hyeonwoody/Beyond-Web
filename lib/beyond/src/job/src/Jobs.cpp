#include "Jobs.h"


Jobs* Jobs::Create() {
    Jobs* jobs = new Jobs();
    jobs->kamsi = Kamsi::Create("Jobs", "Init");
    return jobs;
}

Jobs::~Jobs() {
    for (IJob* job : jobs) {
        delete job;
    }
    delete kamsi;
}

void Jobs::Add(IJob* job) {
    jobMutex.lock();
    jobs.push_back(job);
    jobMutex.unlock();
}

void Jobs::Remove(IJob* job) {
    jobMutex.lock();
    
    auto it = std::find(jobs.begin(), jobs.end(), job);
    if (it != jobs.end()) {
        delete *it;              
        jobs.erase(it);
    }
    jobMutex.unlock();
}

void Jobs::Sort() {
    std::sort(jobs.begin(), jobs.end(),
        [](IJob* x, IJob* y) {
            return x->GetType() < y->GetType();
        }
    );
};



int Jobs::Start() {

    for (IJob* job : jobs) {
        if (pthread_create(&job->thread, nullptr, jobExecutor, job) == 0) {
        } else {
            char buf[64];
            std::snprintf(buf, sizeof(buf), "Failed to create thread for job : %s", JobTypeToString(job->GetType()));
            kamsi->Error("Start", buf);
        }
    }

    for (IJob* job : jobs) {
        pthread_join(job->thread, nullptr);
        this->Remove(job);
    }

    return 0;
}

void* Jobs::jobExecutor(void *arg) {
    IJob* job = static_cast<IJob*>(arg);
    if (job) job->Execute();
    return nullptr;
}
