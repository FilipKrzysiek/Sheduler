//
// Created by krzysiu on 12.05.2020.
//

#ifndef TASKREPEATABLE_H
#define TASKREPEATABLE_H

#include <chrono>

class TaskRepeatable {
public:
    TaskRepeatable(){};

    virtual ~TaskRepeatable(){};
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
    std::chrono::time_point<std::chrono::system_clock> getEndTime();

    /**
     * Return interval executing task.
     * Only for repeatable task.
     * @return time in seconds.
     */
    std::chrono::seconds getInterval();

protected:
    std::chrono::time_point<std::chrono::system_clock> endWorkTime;
    std::chrono::seconds interval;
    bool canSkipped;
    bool isNeverEnding;
};


#endif //TASKREPEATABLE_H
