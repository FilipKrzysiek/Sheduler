//
// Created by krzysiu on 12.05.2020.
//

#ifndef TASKSTATICTIME_H
#define TASKSTATICTIME_H

#include <chrono>

#include "TaskController.h"

class TaskStaticTime : public TaskController {
public:
    /**
     * Create static time task
     * @param task task to execute. Allocate it in constructor. In this object it will be keept in unique ptr
     * @param id task id
     * @param staticExecuteTime time when task should be executed
     * @param blocking if true all tasks run on thread must end before start this task.
     * @param runOnThread run this task on separate thread
     * @param skipOtherTasks true - other repeatable task will be skipped, when this task work, false - other task wait to end work this task
     */
    TaskStaticTime(TaskTypeInterface *task, unsigned int id, tp_system_clock staticExecuteTime, bool blocking = false,
                   bool runOnThread = false, bool skipOtherTasks = true);

    /**
     * Create static time task
     * @param task task to execute. Allocate it in constructor. In this object it will be keept in unique ptr
     * @param id task id
     * @param staticExecuteTime time when task should be executed
     * @param blocking if true all tasks run on thread must end before start this task.
     * @param runOnThread run this task on separate thread
     * @param skipOtherTasks true - other repeatable task will be skipped, when this task work, false - other task wait to end work this task
     */
    TaskStaticTime(unique_ptr<TaskTypeInterface> task, unsigned int id, tp_system_clock staticExecuteTime,
                   bool blocking = false, bool runOnThread = false, bool skipOtherTasks = true);

    /**
     * Return time when task should be executed.
     * Only for static time tasks.
     * @return time
     */
    tp_system_clock getStaticExecuteTime() const;

    /**
     * Check that other repeatable task will be skipped or must wait to end this task. Static time task have higher priority.
     * Only for static time tasks.
     * @return true - other repeatable tasks will be skipped, false - other repeatable task must wait
     */
    bool getSkipOtherTasks() const;

    /**
     * Increment date by 24h
     */
    void incrementStaticExecuteTime();

protected:
    tp_system_clock staticExecuteTime;

    bool skipOtherTasks;
};


#endif //TASKSTATICTIME_H
