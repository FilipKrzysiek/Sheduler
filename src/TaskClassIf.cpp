//
// Created by krzysiu on 21.09.2019.
//

#include "../includes/TaskClassIf.h"

TaskClassIf::TaskClassIf(unsigned int id, unsigned int interval, TaskClassInterface *clss, bool canSkipped, unsigned int endAfter)
{
    this->id = id;
    this->interval = interval;
    this->clss = clss;
    this->canSkipped = canSkipped;
    if(endAfter > 0){
        time(&this->now);
        this->endWorkTime = now + endAfter * 60;
    }
}

void TaskClassIf::execute()
{
    time(&this->now);
    this->lastExecute = now;
    this->clss->execute();
}