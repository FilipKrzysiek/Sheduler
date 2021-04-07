#ifndef TASK_H
#define TASK_H

#include "TaskStaticTime.h"
#include "TaskRepeatable.h"
#include "Exception.h"

class Task : public TaskRepeatable, public TaskStaticTime {
public:
    //Task(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped, unsigned int endAfter){};
    //virtual Task(unsigned int interval, string execString, bool canSkipped = true, unsigned int endAfter = 0);
    Task(const bool runOnThread) : runOnThread(runOnThread) {};

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

    /**
     * Check task is blocking, must wait to end other tasks running on other threads.
     * @return true - task must wait to end all other tasks; false - task don't wait for other tasks
     */
    bool isBlocking() const;

    /**
     * Check task is run on master thread or on separate thread.
     * @return true - task run on separate thread; false - task run on main thread
     */
    bool isRunOnThread();

protected:
    unsigned int id;
    std::chrono::time_point<std::chrono::system_clock> lastExecute;
    bool blocking;
    const bool runOnThread;

};

#endif // TASK_H
