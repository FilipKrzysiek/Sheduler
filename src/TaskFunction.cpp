#include "../includes/TaskFunction.h"

TaskFunction::TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped,
                           std::chrono::seconds endAfter) {
    this->id = id;
    this->interval = interval;
    this->execFun = execFun;
    this->canSkipped = canSkipped;
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter;
        this->isNeverEnding = false;
    } else if (endAfter == 0s) {
        this->isNeverEnding = true;
    } else {
        throw Exception("Passed endAfter less than 0");
    }
}

TaskFunction::TaskFunction(unsigned int id, std::chrono::time_point<std::chrono::system_clock> executeTime,
                           void (*execFun)(), bool skipOtherTasks) {
    this->id = id;
    this->staticExecuteTime = executeTime;
    this->execFun = execFun;
    this->skipOtherTasks = skipOtherTasks;
}


void TaskFunction::execute() {
    this->lastExecute = std::chrono::system_clock::now();
    this->execFun();
}
