#include "Jobs.h"

Jobs* Jobs::Create() {
    return new Jobs();
}

void Jobs::Add(IJob* job) {
    jobs.push_back(job);
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
        job->Execute();
    }
    return 0;
}