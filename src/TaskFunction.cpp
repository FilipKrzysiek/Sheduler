#include "../includes/TaskFunction.h"

TaskFunction::TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped,
                           std::chrono::seconds endAfter) {
    this->id = id;
    this->interval = interval;
    this->execFun = execFun;
    this->canSkipped = canSkipped;
    if (endAfter > 0s) {
        this->endWorkTime = std::chrono::system_clock::now() + endAfter;
        isNeverEnding = false;
    } else if(endAfter == 0s){
        isNeverEnding = true;
    } else {
        throw Exception("Passed endAfter less than 0");
    }
}

void TaskFunction::execute() {
    this->lastExecute = std::chrono::system_clock::now();
    this->execFun();
}
