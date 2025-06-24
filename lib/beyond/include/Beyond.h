#include "Argument.h"
#include "Jobs.h"
#include "Kamsi.h"
class Beyond
{
private:
    Jobs* jobs;
    Kamsi* kamsi;
private:
    Beyond() {};
public:

    ~Beyond();
    static Beyond* Create(ArgumentMapper *argument);
    void pushToJob(CFlag* flag);
    int SortJob();
    int Run();
    int DownStream();

    private:

        
};