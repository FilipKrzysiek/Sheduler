#include "TaskFunction.h"

TaskFunction::TaskFunction(void (*execFun)()): execFun(execFun) {
}


void TaskFunction::execute() {
    execFun();
}
