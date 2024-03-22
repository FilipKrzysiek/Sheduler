//
// Created by krzysiu on 12.05.2020.
//

#include "TaskRepeatable.h"

template<class Rep, class Period>
TaskRepeatable::TaskRepeatable(TaskTypeInterface *task, unsigned int id, std::chrono::duration<Rep, Period> interval,
                               bool blocking, bool runOnThread, bool canBeSkipped,
                               std::chrono::duration<Rep, Period> endAfter)
    : TaskController(task, id, blocking, runOnThread), interval(interval), canBeSkipped(canBeSkipped) {
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter;
        this->isNeverEnding = false;
    } else if (endAfter == 0s) {
        this->isNeverEnding = true;
    } else {
        throw Exception("Passed endAfter less than 0");
    }
}

template<class Rep, class Period>
TaskRepeatable::TaskRepeatable(std::unique_ptr<TaskTypeInterface> task, unsigned int id,
                               std::chrono::duration<Rep, Period> interval, bool blocking, bool runOnThread,
                               bool canBeSkipped,
                               std::chrono::duration<Rep, Period> endAfter)
    : TaskController(std::move(task), id, blocking, runOnThread), interval(interval), canBeSkipped(canBeSkipped) {
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter;
        this->isNeverEnding = false;
    } else if (endAfter == 0s) {
        this->isNeverEnding = true;
    } else {
        throw Exception("Passed endAfter less than 0");
    }
}

std::chrono::time_point<std::chrono::system_clock> TaskRepeatable::getEndTime() {
    return this->endWorkTime;
}

bool TaskRepeatable::getCanSkipped() {
    return this->canBeSkipped;
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
