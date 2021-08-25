//
// Created by krzysiu on 12.05.2020.
//

#include "TaskStaticTime.h"

std::chrono::time_point<std::chrono::system_clock> TaskStaticTime::getStaticExecuteTime() {
    return staticExecuteTime;
}

bool TaskStaticTime::getSkipOtherTasks() const {
    return skipOtherTasks;
}

void TaskStaticTime::incrementStaticExecuteTime() {
    staticExecuteTime = staticExecuteTime + std::chrono::hours(24);

}
