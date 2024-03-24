//
// Created by krzysiu on 12.05.2020.
//

#include "TaskStaticTime.h"

TaskStaticTime::TaskStaticTime(TaskTypeInterface *task, unsigned int id, tp_system_clock staticExecuteTime,
                               bool blocking, bool recalcRepTasks)
    : TaskController(task, id, blocking, !blocking), recalcRepTasks(recalcRepTasks),
      staticExecuteTime(staticExecuteTime) {
}

TaskStaticTime::TaskStaticTime(unique_ptr<TaskTypeInterface> task, unsigned int id, tp_system_clock staticExecuteTime,
                               bool blocking, bool recalcRepTasks)
    : TaskController(std::move(task), id, blocking, !blocking), recalcRepTasks(recalcRepTasks),
      staticExecuteTime(staticExecuteTime) {
}

tp_system_clock TaskStaticTime::getStaticExecuteTime() const {
    return staticExecuteTime;
}

bool TaskStaticTime::getRecalcRepTasks() const {
    return recalcRepTasks;
}

void TaskStaticTime::incrementStaticExecuteTime() {
    staticExecuteTime = staticExecuteTime + std::chrono::hours(24);
}
