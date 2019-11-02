#include "../includes/Task.h"

time_t Task::getLastExecute() {
    return this->lastExecute;
}

time_t Task::getEndTime() {
    return this->endWorkTime;
}

bool Task::getCanSkipped() {
    return this->canSkipped;
}

bool Task::outOfTime() {
    time(&now);
    return now < endWorkTime;
}

unsigned int Task::getId() {
    return this->id;
}

unsigned int Task::getInterval() {
    return interval;
}
/*
Task::~Task()
{
    //dtor
}
*/
