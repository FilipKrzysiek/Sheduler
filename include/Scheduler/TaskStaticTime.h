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
     * @param blocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - execution time of all task will be recalculated (based of actual time), after end this task, if false - all outdated tasks will be skipped.
     */
    TaskStaticTime(TaskTypeInterface *task, unsigned int id, tp_system_clock staticExecuteTime, bool blocking = false,
                   bool recalcRepTasks = false);

    /**
     * Create static time task
     * @param task task to execute. Allocate it in constructor. In this object it will be keept in unique ptr
     * @param id task id
     * @param staticExecuteTime time when task should be executed
     * @param blocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - execution time of all task will be recalculated (based of actual time), after end this task, if false - all outdated tasks will be skipped.
     */
    TaskStaticTime(std::unique_ptr<TaskTypeInterface> task, unsigned int id, tp_system_clock staticExecuteTime,
                   bool blocking = false, bool recalcRepTasks = false);

    /**
     * Return time when task should be executed.
     * Only for static time tasks.
     * @return time
     */
    tp_system_clock getStaticExecuteTime() const;

    /**
     * Get value of flag recalcRepTasks (recalculateRepeatableTasksTime). This option works only when blocking was set to true.
     * RecalcRepTasks flag change behaviour of recalculating repeatable task execution time.
     * @return When true - execution time of all task will be recalculated (based of actual time), after end this task, if false - all outdated tasks will be skipped.
     */
    bool getRecalcRepTasks() const;

    /**
     * Increment date by 24h
     */
    void incrementStaticExecuteTime();

protected:
    tp_system_clock staticExecuteTime;

    bool recalcRepTasks;
};


#endif //TASKSTATICTIME_H
