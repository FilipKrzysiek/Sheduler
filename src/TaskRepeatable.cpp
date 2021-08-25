//
// Created by krzysiu on 12.05.2020.
//

#include "TaskRepeatable.h"

std::chrono::time_point<std::chrono::system_clock> TaskRepeatable::getEndTime() {
    return this->endWorkTime;
}

bool TaskRepeatable::getCanSkipped() {
    return this->canSkipped;
}

bool TaskRepeatable::outOfTime() {
    return std::chrono::system_clock::now() < endWorkTime;
}

std::chrono::seconds TaskRepeatable::getInterval() {
    return interval;
}

bool TaskRepeatable::getIsNeverEnding() {
    return isNeverEnding;
}