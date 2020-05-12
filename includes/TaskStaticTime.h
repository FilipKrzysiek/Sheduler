//
// Created by krzysiu on 12.05.2020.
//

#ifndef TASKSTATICTIME_H
#define TASKSTATICTIME_H

#include <chrono>

class TaskStaticTime {
public:
    /**
     * Return time when task should be executed.
     * Only for static time tasks.
     * @return time
     */
    std::chrono::time_point<std::chrono::system_clock> getStaticExecuteTime();

    /**
     * Check that other repeatable task will be skipped or must wait to end this task. Static time task have higher priority.
     * Only for static time tasks.
     * @return true - other repeatable tasks will be skipped, false - other repeatable task must wait
     */
    bool getSkipOtherTasks() const;

protected:
    std::chrono::time_point<std::chrono::system_clock> staticExecuteTime;

    bool skipOtherTasks;
};


#endif //TASKSTATICTIME_H
