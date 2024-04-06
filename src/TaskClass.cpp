//
// Created by krzysiu on 21.09.2019.
//

#include "TaskClass.h"

TaskClass::TaskClass(TaskClassInterface *clss): clss(clss) {
}

void TaskClass::execute() {
    this->clss->execute();
}
