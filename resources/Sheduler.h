#include <vector>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using system_clock_time  = chrono::time_point<chrono::system_clock>;

class TaskClassInterface {
public:
    /**
     * @brief Execute class method
     */
    virtual void execute() = 0;
};

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

    /**
     * Increment date by 24h
     */
    void incrementStaticExecuteTime();

protected:
    std::chrono::time_point<std::chrono::system_clock> staticExecuteTime;

    bool skipOtherTasks;
};

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
    TaskClass(unsigned int id, std::chrono::time_point<std::chrono::system_clock> executeTime, TaskClassInterface *clss,
              bool skipOtherTasks = true);

    virtual ~TaskClass() {};

    void execute() override;

private:
    TaskClassInterface *clss;
};

class TaskFunction : public Task {
public:
    /**
     * Create repeatable task.
     * @param id task id
     * @param interval time behind this task execution (in seconds)
     * @param execFun address to function which will be executed
     * @param canSkipped Can skipp this task if is delayed
     * @param endAfter if 0s never ending task, else time after which task will end work (not run again).
     */
    TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped = true,
                 std::chrono::seconds endAfter = 0s);

    /**
     * Create static time task
     * @param id task id
     * @param executeTime time when task should be executed
     * @param execFun address to function which will be executed
     * @param skipOtherTasks true - other repeatable task will be skipped, when this task work, false - other task wait to end work this task
     */
    TaskFunction(unsigned int id, std::chrono::time_point<std::chrono::system_clock> executeTime, void (*execFun)(),
                 bool skipOtherTasks = true);

    virtual ~TaskFunction() {};

    void execute() override;

protected:

private:
    void (*execFun)();
};

class SchedulerQueue {
public:
    SchedulerQueue();

    SchedulerQueue(unsigned int planedExec);

    virtual ~SchedulerQueue();

    /**
     * Change pointer to lead on next task.
     * @return true if next task exist, false if not exist next task and can't change pointer
     */
    bool next();

    /**
     * Get actual task
     * @return
     */
    Task *getTask();

    /**
     * Get execute time from actual task
     * @return
     */
    chrono::time_point<chrono::system_clock> getExecTime();

    /**
     * get execute time from next task
     * @return
     */
    chrono::time_point<chrono::system_clock> getNextExecTime();

    /**
     * Get execute time from next the same task as actual task. If not found return time_t null value.
     * @return
     */
    chrono::time_point<chrono::system_clock> getNextThisTaskExecTime();

    /**
     * Check that next task is the same.
     * @return true - next task is the same, false - next task different
     */
    bool nextIsTheSame();

    /**
     * Check is existing next task.
     * @return true - next task is existing, false - next task not exist
     */
    bool nextIsExisting();

    /**
     * Check is existing next task (in all queue) the same as actual task.
     * @return
     */
    bool nextThisTaskIsExisting();

    /**
     * Check existing tasks in queue
     * @return true if exist, false if queue is empty
     */
    bool isExisting();

    /**
     * Set amount the same task plan in queue
     * @param ne amount of planed task (recomended minimum 3)
     */
    void setAmountPlanedExec(unsigned int ne);

    /**
     * Add new task to queue
     * @param tsk address to task
     * @param timeExec time when task should be executed
     */
    void addTask(Task *tsk, chrono::time_point<chrono::system_clock> timeExec);

    /**
     * Print tasks id in queue
     */
    void printTasksId();

    /**
     * Return tasks list with execute time
     * @return
     */
    string getTaskTimeList();

    /**
     * Add actual task to queue
     */
    void addActual();

    /**
     * Remove all tasks from queue.
     */
    void clearQueue();

protected:

private:
    class waitTask {
    public:
        Task *task;
        chrono::time_point<chrono::system_clock> timeExec;
        waitTask *next = nullptr;

        waitTask(Task *task, chrono::time_point<chrono::system_clock> timeExec, waitTask *next = nullptr) {
            this->task = task;
            this->timeExec = timeExec;
            this->next = next;
        }
    };

    waitTask *wTaskActual = nullptr;
    bool nextExisting = false;
    unsigned int planedExec;
};


class Scheduler {
public:
    Scheduler();

    /**
     * Create sheduler and set time after app wnd work.
     * @param endAfterMinutes
     */
    Scheduler(chrono::minutes endAfterMinutes);

    virtual ~Scheduler();

    //TODO Documentation
    /**
     * Adding new task to que.
     * @param interval Interval of calling passed function.
     * @param execFun Address for function to call (execute).
     * @param canSkipped Call can be skipped when is delayed.
     * @param endAfter Ending after seconds. If 0 never ending.
     */
    void addNewTask(chrono::seconds interval, void (*execFun)(), bool canSkipped = true, chrono::seconds endAfter = 0s);

    /**
     * Adding new task to que.
     * @param interval Interval of calling passed function.
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param canSkipped Call can be skipped when is delayed.
     * @param endAfter Ending after seconds. If 0 never ending.
     */
    void addNewTask(chrono::seconds interval, TaskClassInterface *clss, bool canSkipped = true,
                    chrono::seconds endAfter = 0s);


    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(), bool skippOtherTasks = true);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss, bool skippOtherTasks = true);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool skippOtherTasks = true);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool skippOtherTasks = true);

    /**
     * Add new task executing addTask specific time
     * @param hour Hour when task must be executed.
     * @param minute Minute when task must be executed.
     * @param second Second when task must be executed.
     * @param execFun Address for function to call (execute).
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second, void (*execFun)(),
                             bool skippOtherTasks = true);

    /**
     * Add new task executing addTask specific time
     * @param hour Hour when task must be executed.
     * @param minute Minute when task must be executed.
     * @param second Second when task must be executed.
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                             TaskClassInterface *clss, bool skippOtherTasks = true);


    /**
     * Set after how many minutes sheduler must end work. If not set sheduler never ending.
     * @param minutes Minutes from begin to end sheduler work
     */
    void setEndWorkTimeAfter(chrono::minutes minutes);

    /**
     * Set datetime when sheduler muest end work. If not set sheduler never ending.
     * @param date Date when sheduler must end work time
     * @param getOnlyTime Get only time, not date of end work. Default get only time (true).
     */
    void setEndWorkTime(system_clock_time date, bool getOnlyTime = true);

    /**
     * Set datetime when scheduler must end work. If not set scheduler never ending.
     * @param hour Hour at scheduler must end work.
     * @param minute Minute of passed hour, when scheduler must end work.
     * @param second Second of passed minute, when scheduler must end work.
     * @param year Year when scheduler must end work (if not passed get this year).
     * @param month Month of passed year, when scheduler must end work (if not passed get this month).
     * @param day Day of passed month, when scheduler must end work (if not passed get this day).
     */
    void setEndWorkTime(unsigned short hour, unsigned short minute, unsigned short second,
                        unsigned short year = 65535, unsigned short month = 65535, unsigned short day = 65535);

    /**
     * Set datetime when scheduler muest end work. If not set scheduler never ending.
     * @param date Date when scheduler must end work time.
     * @param getOnlyTime Get only time, not date of end work. Default get only time (true).
     */
    void setEndWorkTime(time_t date, bool getOnlyTime = true);

    /**
     * @brief Set max time gap between tasks. If time gap is higher than declared throw error.
     * @param minutes max value 70
     */
    void setMaxTimeGap(chrono::minutes minutes);

    /**
     * Set flag end when repeatable task queue is empty or repeatable task and static time task queue is empty.
     * Default ending when repeatable task queue is empty.
     * @param flg true - end when repeatable task queue is empty, false - end when repeatable and static time task queue is empty
     */
    void setEndWhenRepeatableEnd(bool flg);

    void setDelayBetweenTasks(const chrono::milliseconds &delayBetweenTasks);

    /**
     * @brief Run sheduler, start executing tasks.
     */
    void run();

private:
    //time_t endWorkingTime, now, slept, maxTimeGap = 1000 * 60 * 10;
    system_clock_time endWorkingTime, now;
    chrono::milliseconds slept, delayBetweenTasks = 1s;
    chrono::seconds maxTimeGap = 24h + 1min;
    vector<Task *> repeatableTaskList;
    vector<Task *> staticTimeTaskList;
    SchedulerQueue schedulerQueueRepeatable, schedulerQueueStaticTime;
    unsigned int taskId;
    unsigned int planedExec;
    bool flgEndWorkTimeEnabled = false;
    bool flgEndWhenRepeatableEnd = true;

    /**
     *
     */
    void prepareRun();

    void prepareRunRepeatable();

    void prepareRunStaticTime();

    void runLoop();

    /**
     *
     * @return true if static time task was runned, false if not runned
     */
    bool runStaticTime();

    void runRepeatable();

    void skippingTasks();

    void executeRepeatableTask();

    bool checkCorrectTime(unsigned short hour, unsigned short minute, unsigned short second);

    bool checkCorrectDate(unsigned short year, unsigned short month, unsigned short day);

    time_t calculateEndTime(time_t base, unsigned short hour, unsigned short minute, unsigned short second);

    chrono::milliseconds calcSleepTime();

    system_clock_time calculateExecuteTime(system_clock_time executeTime);

    string getTaskTimeList();

    chrono::milliseconds getSleept();
};

