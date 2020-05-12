#include "../includes/Task.h"

std::chrono::time_point<std::chrono::system_clock> Task::getLastExecute() {
    return this->lastExecute;
}

unsigned int Task::getId() {
    return this->id;
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

std::chrono::seconds Task::getInterval() {
    return interval;
}

bool Task::getIsNeverEnding() {
    return isNeverEnding;
}

std::chrono::time_point<std::chrono::system_clock> Task::getStaticExecuteTime() {
    return staticExecuteTime;
}
