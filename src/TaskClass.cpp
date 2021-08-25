//
// Created by krzysiu on 21.09.2019.
//

#include "TaskClass.h"

TaskClass::TaskClass(unsigned int id, std::chrono::seconds interval, TaskClassInterface *clss, bool canSkipped,
                     bool isBlocking, bool runOnThread, std::chrono::seconds endAfter) : Task(runOnThread)  {
    this->id = id;
    this->interval = interval;
    this->clss = clss;
    this->canSkipped = canSkipped;
    this->blocking = isBlocking;
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter;
        this->isNeverEnding = false;
    } else if (endAfter == 0s) {
        this->isNeverEnding = true;
    } else {
        throw Exception("Passed endAfter less than 0");
    }
}

TaskClass::TaskClass(unsigned int id, std::chrono::time_point<std::chrono::system_clock> executeTime,
                     TaskClassInterface *clss,
                     bool skipOtherTasks, bool isBlocking, bool runOnThread) : Task(runOnThread)  {
    this->id = id;
    this->staticExecuteTime = executeTime;
    this->clss = clss;
    this->skipOtherTasks = skipOtherTasks;
    this->blocking = isBlocking;
}

void TaskClass::execute() {
    this->lastExecute = std::chrono::system_clock::now();
    this->clss->execute();
}
