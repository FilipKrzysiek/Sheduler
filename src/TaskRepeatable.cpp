//
// Created by krzysiu on 12.05.2020.
//

#include "TaskRepeatable.h"

TaskRepeatable::TaskRepeatable(TaskTypeInterface *task, unsigned int id, std::chrono::microseconds interval,
                               bool blocking, bool runOnThread, bool canBeSkipped,
                               std::chrono::microseconds endAfter)
    : TaskController(task, id, blocking, runOnThread), interval(interval), canBeSkipped(canBeSkipped) {
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter;
        this->isNeverEnding = false;
    } else if (endAfter == 0s) {
        this->isNeverEnding = true;
    } else {
        //TODO static_assert
        throw scheduler::exception("Passed endAfter less than 0");
    }
}

TaskRepeatable::TaskRepeatable(std::unique_ptr<TaskTypeInterface> task, unsigned int id,
                               std::chrono::microseconds interval, bool blocking, bool runOnThread,
                               bool canBeSkipped,
                               std::chrono::microseconds endAfter)
    : TaskController(std::move(task), id, blocking, runOnThread), interval(interval), canBeSkipped(canBeSkipped) {
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter;
        this->isNeverEnding = false;
    } else if (endAfter == 0s) {
        this->isNeverEnding = true;
    } else {
        //TODO static_assert
        throw scheduler::exception("Passed endAfter less than 0");
    }
}

std::chrono::time_point<std::chrono::system_clock> TaskRepeatable::getEndTime() {
    return endWorkTime;
}

bool TaskRepeatable::getCanSkipped() {
    return canBeSkipped;
}

bool TaskRepeatable::outOfTime() {
    if (isNeverEnding) return false;
    return std::chrono::system_clock::now() > endWorkTime;
}

std::chrono::microseconds TaskRepeatable::getInterval() {
    return interval;
}

bool TaskRepeatable::getIsNeverEnding() {
    return isNeverEnding;
}
