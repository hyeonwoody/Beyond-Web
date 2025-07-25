#include "Beyond.h"
#include "Jobs.h"
#include <cstring>
#include <stdio.h>

Beyond* Beyond::Create(ArgumentMapper* argument) {
    Beyond* beyond = new Beyond();
    Kamsi* kamsi = Kamsi::Create("Init");
    

    CFlag** pFlag = argument->GetFlags();
    
    JobFactory* pJobFactory = JobFactory::Create();
    Jobs* pJobs = Jobs::Create();

    for (int i = 0; i < FLAG_NUM; ++i) {
        if (pFlag[i]->used) {
            
            kamsi->Debug(beyond->getClassName(), "Create", "argc : " + pFlag[i]->shortName);
            IJob* pJob = pJobFactory->CreateJob(pFlag[i]);
            if (pJob == nullptr) {
                kamsi->Warning(beyond->getClassName(), "Create", "Corresponding job not found. Skipping current job and moving to the next one.");
                continue;
            }
                
            pJob->SetVariables(argument->GetOptionVariables(), argument->GetFlagVariables());
            pJobs->Add(pJob);
        }
    }

    COption** pOption = argument->GetOptions();
    for (int i = 0; i <OPTION_NUM; ++i) {
        if (pOption[i]->used) {
            kamsi->Debug(beyond->getClassName(), "Create", "argc : " + pOption[i]->shortName);
        }
    }
    delete pJobFactory;
    beyond->jobs = pJobs;
    beyond->kamsi = kamsi;
    beyond->kamsi->Register();
    return beyond;
}

Beyond::~Beyond() {
    delete jobs;
    if (kamsi) {
        kamsi->UnRegister();
        kamsi = nullptr;
    }
}

int Beyond::SortJob() {
    jobs->Sort();
    return 0;
}

int Beyond::Run()
{
    int ret = jobs->Start();
    return ret;
}

void Usage() {
    std::cout<<"📢Usage: Beyond -[Option Parameter] -[Flag Parameter]"<<std::endl;
}

int main (int argc, char **argv) {
     
    ArgumentMapper* pArgument = ArgumentMapper::Create();
    pArgument->Parse(argc, argv);

    Beyond* beyond = Beyond::Create(pArgument);

    delete pArgument;

    beyond->SortJob();

    beyond->Run();
    delete beyond;
    return 0;
}