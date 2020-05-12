#ifndef TASK_H
#define TASK_H

class Task {
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

    /**
     * Return time when task should be executed.
     * Only for static time tasks.
     * @return time
     */
    std::chrono::time_point<std::chrono::system_clock> getStaticExecuteTime();

protected:
    unsigned int id;
    std::chrono::time_point<std::chrono::system_clock> endWorkTime, lastExecute, staticExecuteTime;
    std::chrono::seconds interval;
    bool canSkipped;
    bool isNeverEnding;
    bool skipOtherTasks;

};

#endif // TASK_H
