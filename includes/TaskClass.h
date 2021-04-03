//
// Created by krzysiu on 21.09.2019.
//

#ifndef TASKCLASSIF_H
#define TASKCLASSIF_H

#include <chrono>
#include "TaskClassInterface.h"
#include "Task.h"


using namespace std::chrono_literals;


class TaskClass : public Task {
public:
    /**
     * Create repeatable task.
     * @param id task id
     * @param interval time behind this task execution (in seconds)
     * @param clss address to class which will have method execute (extended by TaskClassInterface) to execute task
     * @param canSkipped Can skipp this task if is delayed
     * @param endAfter if 0s never ending task, else time after which task will end work (not run again).
     */
    TaskClass(unsigned int id, std::chrono::seconds interval, TaskClassInterface *clss, bool canSkipped = true,
              std::chrono::seconds endAfter = 0s);

    /**
     * Create static time task
     * @param id task id
     * @param executeTime time when task should be executed
     * @param clss address to class which will have method execute (extended by TaskClassInterface) to execute task
     * @param skipOtherTasks true - other repeatable task will be skipped, when this task work, false - other task wait to end work this task
     */
    TaskClass(unsigned int id, std::chrono::time_point<std::chrono::system_clock> executeTime,
              TaskClassInterface *clss,
              bool skipOtherTasks = true);

    virtual ~TaskClass() {};

    void execute() override;

private:
    TaskClassInterface *clss;
};

#endif //TASKCLASSIF_H
