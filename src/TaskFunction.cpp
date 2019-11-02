#include "../includes/TaskFunction.h"

TaskFunction::TaskFunction(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped, unsigned int endAfter)
{
    this->id = id;
    this->interval = interval;
    this->execFun = execFun;
    this->canSkipped = canSkipped;
    if(endAfter > 0){
        time(&this->now);
        this->endWorkTime = now + endAfter * 60;
    }
}

void TaskFunction::execute()
{
    time(&this->now);
    this->lastExecute = now;
    this->execFun();
}
