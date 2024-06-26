//
// Created by krzysiu on 12.05.2020.
//

#ifndef TASKREPEATABLE_H
#define TASKREPEATABLE_H

#include "TaskController.h"

class TaskRepeatable : public TaskController {
public:
    /**
     * Create repeatable task
     * @tparam Rep see chrono::duration
     * @tparam Period see chrono::duration
     * @param task task to execute. Allocate it in constructor. In this object it will be keept in unique ptr
     * @param id task id
     * @param interval time behind this task execution (in seconds)
     * @param blocking if true all tasks run on thread must end before start this task.
     * @param runOnThread run this task on separate thread.
     * @param canBeSkipped Can skipp this task if is delayed
     * @param endAfter if 0s never ending task, else time after which task will end work (not run again).
     */
    TaskRepeatable(TaskTypeInterface *task, unsigned int id, std::chrono::microseconds interval,
                   bool blocking = false, bool runOnThread = false, bool canBeSkipped = false,
                   std::chrono::microseconds endAfter = 0s);

    /**
     * Create repeatable task
     * @tparam Rep see chrono::duration
     * @tparam Period see chrono::duration
     * @param task task to execute
     * @param id task id
     * @param interval time behind this task execution (in seconds)
     * @param blocking if true all tasks run on thread must end before start this task.
     * @param runOnThread run this task on separate thread.
     * @param canBeSkipped Can skipp this task if is delayed
     * @param endAfter if 0s never ending task, else time after which task will end work (not run again).
     */
    TaskRepeatable(std::unique_ptr<TaskTypeInterface> task, unsigned int id, std::chrono::microseconds interval,
                   bool blocking = false, bool runOnThread = false, bool canBeSkipped = false,
                   std::chrono::microseconds endAfter = 0s);

    virtual ~TaskRepeatable() {
    };

    /**
     * Check task can be skipped.
     * Only for repeatable task.
     * @return
     */
    bool getCanSkipped();

    /**
     * Check task end time and check that is this task should be ended. (End time task is in the past.)
     * Only for repeatable task.
     * @return true if end time is in the past
     */
    bool outOfTime();

    /**
     * Get information about task have end work time, or is never ending task.
     * Only for repeatable task.
     * @return true - task is never ending, false - task have end work time.
     */
    bool getIsNeverEnding();

    /**
     * Return end work time of task.
     * Only for repeatable task.
     * @return
     */
    tp_system_clock getEndTime();

    /**
     * Return interval executing task.
     * Only for repeatable task.
     * @return time in microseconds.
     */
    std::chrono::microseconds getInterval();

protected:
    tp_system_clock endWorkTime;
    std::chrono::microseconds interval;
    bool canBeSkipped;
    bool isNeverEnding;
};


#endif //TASKREPEATABLE_H
