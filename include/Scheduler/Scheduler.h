#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include "TaskClassInterface.h"
#include "TaskClass.h"
#include "TaskFunction.h"
#include "Exception.h"
#include "SchedulerQueue.h"

using namespace std;
using namespace std::chrono_literals;
using system_clock_time = chrono::time_point<chrono::system_clock>;


class Scheduler {
public:
    Scheduler();

    /**
     * Create sheduler and set time after app wnd work.
     * @param endAfter
     */
    template<class Rep, class Period>
    explicit Scheduler(chrono::duration<Rep, Period> endAfter);

    virtual ~Scheduler();

    //TODO Documentation
    /**
     * Adding new task to que.
     * @param interval Interval of calling passed function.
     * @param execFun Address for function to call (execute).
     * @param runOnThread run this task on thread
     * @param canSkipped Call can be skipped when is delayed.
     * @param isBlocking if true all tasks run on thread must end before start this task.
     * @param endAfter Ending after seconds. If 0 never ending.
     */
    template<class Rep, class Period>
    void addNewTask(chrono::duration<Rep, Period> interval, void (*execFun)(), bool runOnThread = true,
                    bool canSkipped = true, bool isBlocking = false, chrono::duration<Rep, Period> endAfter = 0s);

    /**
     * Adding new task to que.
     * @param interval Interval of calling passed function.
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param runOnThread run this task on thread
     * @param canSkipped Call can be skipped when is delayed.
     * @param isBlocking if true all tasks run on thread must end before start this task.
     * @param endAfter Ending after seconds. If 0 never ending.
     */
    template<class Rep, class Period>
    void addNewTask(chrono::duration<Rep, Period> interval, TaskClassInterface *clss, bool runOnThread = true,
                    bool canSkipped = true, bool isBlocking = false, chrono::duration<Rep, Period> endAfter = 0s);


    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param runOnThread run this task on thread
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     * @param isBlocking if true all tasks run on thread must end before start this task.
     */
    void addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(), bool runOnThread = true,
                             bool skippOtherTasks = true, bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param runOnThread run this task on thread
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     * @param isBlocking if true all tasks run on thread must end before start this task.
     */
    void addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss, bool runOnThread = true,
                             bool skippOtherTasks = true, bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param runOnThread run this task on thread
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     * @param isBlocking if true all tasks run on thread must end before start this task.
     */
    void addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool runOnThread = true,
                             bool skippOtherTasks = true, bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param runOnThread run this task on thread
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     * @param isBlocking if true all tasks run on thread must end before start this task.
     */
    void addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool runOnThread = true,
                             bool skippOtherTasks = true, bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param hour Hour when task must be executed.
     * @param minute Minute when task must be executed.
     * @param second Second when task must be executed.
     * @param execFun Address for function to call (execute).
     * @param runOnThread run this task on thread
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     * @param isBlocking if true all tasks run on thread must end before start this task.
     */
    void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second, void (*execFun)(),
                             bool runOnThread = true, bool skippOtherTasks = true, bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param hour Hour when task must be executed.
     * @param minute Minute when task must be executed.
     * @param second Second when task must be executed.
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param runOnThread run this task on thread
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     * @param isBlocking if true all tasks run on thread must end before start this task.
     */
    void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                             TaskClassInterface *clss, bool runOnThread = true, bool skippOtherTasks = true,
                             bool isBlocking = false);

    //TODO make constexpr

    /**
     * Set after how many minutes sheduler must end work. If not set sheduler never ending.
     * @param time Time from begin to end sheduler work. It schould be chrono time value (eg. chrono::minutes)
     */
    template<class Rep, class Period>
    void setEndWorkTimeAfter(chrono::duration<Rep, Period> time);

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
     * @param time in chrono time value (eg. chrono::minutes) default 1h
     */
    template<class Rep, class Period>
    void setMaxTimeGap(chrono::duration<Rep, Period> time);

    /**
     * Set flag end when repeatable task queue is empty or repeatable task and static time task queue is empty.
     * Default ending when repeatable task queue is empty.
     * @param flg true - end when repeatable task queue is empty, false - end when repeatable and static time task queue is empty
     */
    void setEndWhenRepeatableEnd(bool flg);

    //TODO change name

    /**
     * Set delay between tasks on theirs first run (that all task not started at the same time, because second, third...
     * task was delayed when they started).
     * Default 1s.
     * @param delayBetweenTasks time in chrono rime value (eg. chrono::seconds)
     */
    template<class Rep, class Period>
    void setDelayBetweenTasks(const chrono::duration<Rep, Period> time &delayBetweenTasks);

    /**
     * @brief Run sheduler, start executing tasks.
     */
    void run();

    //TODO stop

private:
    class TaskOnThread {
    public:
        explicit TaskOnThread(TaskController *task);

        virtual ~TaskOnThread();

        bool taskEnd() const;

        unsigned int getTaskId() const;

    private:
        thread th;
        bool working = true;
        TaskController *task;

        void threadTask();
    };

    struct TaskListItem {
        TaskController *task;
        system_clock_time execTime;
    };

    system_clock_time endWorkingTime, now;
    chrono::microseconds slept = 0ms;
    chrono::microseconds delayBetweenTasks = 1s;
    chrono::seconds maxTimeGap = 1h;
    const chrono::milliseconds waitForEndTaskOnThreadTime = 5ms;
    vector<unique_ptr<TaskRepeatable> > repeatableTaskList;
    vector<unique_ptr<TaskStaticTime> > staticTimeTaskList;
    vector<TaskOnThread *> runningTaskOnThread;
    SchedulerQueue schedulerQueueRepeatable, schedulerQueueStaticTime;

    unsigned int taskId;
    unsigned int planedExec;
    thread taskCollector;
    mutex runningTaskOnThreadLocker;
    bool flgEndWorkTimeEnabled = false;
    bool flgEndWhenRepeatableEnd = true;
    bool flgIsAnyTaskOnThread = false;
    bool flgEndTaskCollector = false;

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

    void executeTask(TaskController *task);

    bool checkCorrectTime(unsigned short hour, unsigned short minute, unsigned short second);

    bool checkCorrectDate(unsigned short year, unsigned short month, unsigned short day);

    inline bool checkEndLoop(SchedulerQueue *repeatable, SchedulerQueue *staticTime);

    time_t calculateEndTime(time_t base, unsigned short hour, unsigned short minute, unsigned short second);

    chrono::milliseconds calcSleepTime();

    system_clock_time calculateExecuteTime(system_clock_time executeTime);

    string getTaskTimeList();

    chrono::microseconds getSleept();

    bool thisTaskIsRunning(unsigned int id);

    void taskCollectorFunction();
};

#endif // SCHEDULER_H
