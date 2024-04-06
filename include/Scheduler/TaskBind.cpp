//
// Created by Filip on 29.03.2024.
//

#include "TaskBind.h"

TaskBind::TaskBind(const TaskBind &other): TaskTypeInterface(other),
                                           callbackElement(other.callbackElement) {
}

TaskBind::TaskBind(TaskBind &&other) noexcept: TaskTypeInterface(std::move(other)),
                                               callbackElement(std::move(other.callbackElement)) {
}

void TaskBind::execute() {
    callbackElement();
}
