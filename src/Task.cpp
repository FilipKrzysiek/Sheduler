#include "../includes/Task.h"

std::chrono::time_point<std::chrono::system_clock> Task::getLastExecute() {
    return this->lastExecute;
}

std::chrono::time_point<std::chrono::system_clock> Task::getEndTime() {
    return this->endWorkTime;
}

bool Task::getCanSkipped() {
    return this->canSkipped;
}

bool Task::outOfTime() {
    return std::chrono::system_clock::now() < endWorkTime;
}

unsigned int Task::getId() {
    return this->id;
}

std::chrono::seconds Task::getInterval() {
    return interval;
}
/*
Task::~Task()
{
    //dtor
}
*/
