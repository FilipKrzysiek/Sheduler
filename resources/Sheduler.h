#include <vector>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

class TaskClassInterface {
public:
    /**
     * @brief Execute class method
     */
    virtual void execute() = 0;
};

class Task {
public:
    //Task(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped, unsigned int endAfter){};
    //virtual Task(unsigned int interval, string execString, bool canSkipped = true, unsigned int endAfter = 0);
    Task() {};

    virtual ~Task() {};

    virtual void execute() = 0;

    //Getters
    std::chrono::time_point<std::chrono::system_clock> getLastExecute();

    bool getCanSkipped();

    /**
     * @brief Check pass end work time
     * @return
     */
    bool outOfTime();

    bool getIsNeverEnding();

    std::chrono::time_point<std::chrono::system_clock> getEndTime();

    unsigned int getId();

    std::chrono::seconds getInterval();

protected:
    unsigned int id;
    std::chrono::seconds interval;
    bool canSkipped;
    bool isNeverEnding;
    std::chrono::time_point<std::chrono::system_clock> endWorkTime, lastExecute;
private:

    void (*taskFun)();

};

class TaskClass : public Task {
public:
    TaskClass(unsigned int id, std::chrono::seconds interval, TaskClassInterface *clss, bool canSkipped = true,
              std::chrono::seconds endAfter = 0s);

    virtual ~TaskClass() {};

    void execute() override;

private:
    TaskClassInterface *clss;
};

class TaskFunction : public Task {
public:
    TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped = true,
                 std::chrono::seconds endAfter = 0s);

    virtual ~TaskFunction() {};

    void execute() override;

private:
    void (*execFun)();
};

class ShedulerList {
public:
    ShedulerList();

    ShedulerList(unsigned int planedExec);

    virtual ~ShedulerList();

    bool next();

    Task *getTask();

    chrono::time_point<chrono::system_clock> getExecTime();

    chrono::time_point<chrono::system_clock> getNextExecTime();

    bool nextIsTheSame();

    bool nextIsExisting();

    bool isExisting();

    void setAmountPlanedExec(unsigned int ne);

    void add(Task *tsk, chrono::time_point<chrono::system_clock> timeExec);

    void printTasksId();

    string getTaskTimeList();

    void addActual();

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
    bool nextExisting;
    unsigned int planedExec;
};


class Sheduler {
public:
    Sheduler();

    /**
     * Create sheduler and set time after app wnd work.
     * @param endAfterMinutes
     */
    Sheduler(chrono::minutes endAfterMinutes);

    virtual ~Sheduler() {}

    //TODO Exceptions as exception, not as const char*
    //TODO Overwrite method setEndWorkTime to pass year, mont, day, hour, minute, second and hour, minute, second
    //TODO Documentation
    /**
     * Adding new task to que.
     * @param interval Interval of calling passed function.
     * @param execFun Address for function to call.
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


//    void
//    addNewTaskCallingAt(unsigned int interval, void (*execFun)(), bool canSkipped = true, unsigned int endAfter = 0);
//
//    void addNewTaskCallingAt(unsigned int interval, TaskClassInterface *clss, bool canSkipped = true,
//                             unsigned int endAfter = 0);

    /**
     * Set after how many minutes sheduler must end work. If not setted sheduler never ending.
     * @param minutes Minutes from begin to end sheduler work
     */
    void setEndWorkTimeAfter(chrono::minutes minutes);

    /**
     * Set datetime when sheduler muest end work. If not setted sheduler never ending.
     * @param date Date when sheduler must end work time
     */
    void setEndWorkTime(chrono::time_point<chrono::system_clock> date);

    /**
     * @brief Set max time gap between tasks. If time gap is higher than declared throw error.
     * @param minutes max value 70
     */
    void setMaxTimeGap(chrono::minutes minutes);

    /**
     * Set delay between tasks on theirs first run (that all task not started at the same time, because second, third...
     * task was delayed when they started).
     * Default 1s.
     * @param milliseconds time between tasks
     */
    void setDelayBetweenTasks(chrono::milliseconds milliseconds);

    /**
     * @brief Run sheduler, start executing tasks.
     */
    void run();

private:
    //time_t endWorkingTime, now, slept, maxTimeGap = 1000 * 60 * 10;
    chrono::time_point<chrono::system_clock> endWorkingTime, now;
    chrono::milliseconds slept = 0ms, delayBetweenTasks = 1s;
    chrono::seconds  maxTimeGap = 10min;
    vector<Task *> taskList;
    ShedulerList shList;
    unsigned int taskId;
    unsigned int planedExec;
    bool flgEndWorkTimeEnabled = false;

    /**
     *
     * @param delayBetweenTasks - seconds betwen first runs tasks
     */
    void prepareRun();

    void runLoop();

    void executeTask();

    chrono::milliseconds calcSleepTime(chrono::time_point<chrono::system_clock> execTime);

    string getTaskTimeList();

    chrono::milliseconds getSleept();
};

