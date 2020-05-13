#ifndef TASK_H
#define TASK_H

#include "TaskStaticTime.h"
#include "TaskRepeatable.h"

class Task: public TaskRepeatable, public TaskStaticTime {
public:
    //Task(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped, unsigned int endAfter){};
    //virtual Task(unsigned int interval, string execString, bool canSkipped = true, unsigned int endAfter = 0);
    Task() {};

    virtual ~Task() {};

    /**
     * Execute method, this must be overrided.
     */
    virtual void execute() = 0;

    /**
     * Get last execute task time.
     * @return Time of last execute
     */
    std::chrono::time_point<std::chrono::system_clock> getLastExecute();

    /**
     * Get task id.
     * @return id task
     */
    unsigned int getId();

protected:
    unsigned int id;
    std::chrono::time_point<std::chrono::system_clock> lastExecute;

};

#endif // TASK_H
