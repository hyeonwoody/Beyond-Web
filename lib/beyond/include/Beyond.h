#include "Argument.h"
#include "Jobs.h"
class Beyond
{
private:
    Jobs* jobs;
public:
    Beyond();
    
    static Beyond* Create(CArgumentMapper *argument);
    void pushToJob(CFlag* flag);
    int SortJob();
    int Run();
    int DownStream();

    private:

        
};