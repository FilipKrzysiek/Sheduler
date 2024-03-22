//
// Created by krzysiu on 12.05.2020.
//

#include "TaskStaticTime.h"

TaskStaticTime::TaskStaticTime(TaskTypeInterface *task, unsigned int id, tp_system_clock staticExecuteTime,
                               bool blocking, bool runOnThread, bool skipOtherTasks)
    : TaskController(task, id, blocking, runOnThread), skipOtherTasks(skipOtherTasks),
      staticExecuteTime(staticExecuteTime) {
}

TaskStaticTime::TaskStaticTime(unique_ptr<TaskTypeInterface> task, unsigned int id, tp_system_clock staticExecuteTime,
                               bool blocking, bool runOnThread, bool skipOtherTasks)
    : TaskController(std::move(task), id, blocking, runOnThread), skipOtherTasks(skipOtherTasks),
      staticExecuteTime(staticExecuteTime) {
}

tp_system_clock TaskStaticTime::getStaticExecuteTime() const {
    return staticExecuteTime;
}

bool TaskStaticTime::getSkipOtherTasks() const {
    return skipOtherTasks;
}

void TaskStaticTime::incrementStaticExecuteTime() {
    staticExecuteTime = staticExecuteTime + std::chrono::hours(24);
}
