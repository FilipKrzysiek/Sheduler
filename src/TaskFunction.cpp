#include "../includes/TaskFunction.h"

TaskFunction::TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped,
                           std::chrono::seconds endAfter) {
    this->id = id;
    this->interval = interval;
    this->execFun = execFun;
    this->canSkipped = canSkipped;
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter * 60;
    }
}

void TaskFunction::execute() {
    this->lastExecute = std::chrono::system_clock::now();
    this->execFun();
}
