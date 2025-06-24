#include "Beyond.h"
#include "Jobs.h"
#include <cstring>
#include <stdio.h>

Beyond::Beyond() {}

Beyond* Beyond::Create(CArgumentMapper* argument) {
    Beyond* beyond = new Beyond();
    Kamsi* kamsi = Kamsi::Create("Beyond", "Init");
    

    CFlag** pFlag = argument->GetFlags();
    
    JobFactory* pJobFactory = JobFactory::Create();
    Jobs* pJobs = Jobs::Create();

    for (int i = 0; i < FLAG_NUM; ++i) {
        if (pFlag[i]->used) {
            
            kamsi->Debug("Create", "argc : " + pFlag[i]->shortName);
            IJob* pJob = pJobFactory->CreateJob(pFlag[i]);
            if (pJob == nullptr) {
                kamsi->Warning("Create", "Corresponding job not found. Skipping current job and moving to the next one.");
                continue;
            }
                
            pJob->SetVariables(argument->GetOptionVariables(), argument->GetFlagVariables());
            pJobs->Add(pJob);
        }
    }

    COption** pOption = argument->GetOptions();
    for (int i = 0; i <OPTION_NUM; ++i) {
        if (pOption[i]->used) {
            kamsi->Debug("Create", "argc : " + pOption[i]->shortName);
        }
    }
    
    beyond->jobs = pJobs;
    beyond->kamsi = kamsi;
    return beyond;
}

int Beyond::SortJob() {
    jobs->Sort();
    return 0;
}

int Beyond::Run()
{
    int ret = jobs->Start();
    return 0;
}

void Usage() {
    std::cout<<"📢Usage: Beyond -[Option Parameter] -[Flag Parameter]"<<std::endl;
}

int main (int argc, char **argv) {
     
    CArgumentMapper* pArgument = CArgumentMapper::Create();
    pArgument->Parse(argc, argv);

    Beyond* beyond = Beyond::Create(pArgument);

    delete pArgument;

    beyond->SortJob();

    beyond->Run();
    delete beyond;
    return 0;
}