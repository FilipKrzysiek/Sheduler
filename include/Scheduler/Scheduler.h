#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <deque>
#include <thread>
#include <algorithm>
#include <future>
#include <map>
#include "TaskRepeatable.h"
#include "TaskStaticTime.h"

#include "TaskClassInterface.h"
#include "TaskFunction.h"
#include "TaskClass.h"
#include "Exception.h"

using namespace std;
using namespace std::chrono_literals;
using system_clock_time = chrono::time_point<chrono::system_clock>;

//TODO delete ended task from list
//TODO pass class method like to std::thread
//TODO add lambda

class Scheduler {
public:
    Scheduler();

    /**
     * Create sheduler and set time after app wnd work.
     * @param endAfter
     */
    explicit Scheduler(chrono::microseconds endAfter);

    virtual ~Scheduler();

    //TODO Documentation
    /**
     * Adding new task to que.
     * @param interval Interval of calling passed function.
     * @param execFun Address for function to call (execute).
     * @param runOnThread run this task on thread
     * @param canSkipped Call can be skipped when is delayed or when the same task still work.
     * @param isBlocking if true all tasks run on thread must end before start this task.
     * @param endAfter Ending after seconds. If 0 never ending.
     */
    void addNewTask(chrono::microseconds interval, void (*execFun)(), bool runOnThread = true,
                    bool canSkipped = true, bool isBlocking = false, chrono::microseconds endAfter = 0s);

    /**
     * Adding new task to que.
     * @param interval Interval of calling passed function.
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param runOnThread run this task on thread
     * @param canSkipped Call can be skipped when is delayed or when the same task still work.
     * @param isBlocking if true all tasks run on thread must end before start this task.
     * @param endAfter Ending after seconds. If 0 never ending.
     */
    void addNewTask(chrono::microseconds interval, TaskClassInterface *clss, bool runOnThread = true,
                    bool canSkipped = true, bool isBlocking = false, chrono::microseconds endAfter = 0s);


    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - all outdated tasks will be skipped, if false - execution time of all task will be recalculated, after end this task.
     * @param isBlocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     */
    void addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(), bool recalcRepTasks = true,
                             bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - all outdated tasks will be skipped, if false - execution time of all task will be recalculated, after end this task.
     * @param isBlocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     */
    void addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss, bool recalcRepTasks = true,
                             bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - all outdated tasks will be skipped, if false - execution time of all task will be recalculated, after end this task.
     * @param isBlocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     */
    void addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool recalcRepTasks = true,
                             bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - all outdated tasks will be skipped, if false - execution time of all task will be recalculated, after end this task.
     * @param isBlocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     */
    void addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool recalcRepTasks = true,
                             bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param hour Hour when task must be executed.
     * @param minute Minute when task must be executed.
     * @param second Second when task must be executed.
     * @param execFun Address for function to call (execute).
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - all outdated tasks will be skipped, if false - execution time of all task will be recalculated, after end this task.
     * @param isBlocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     */
    void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second, void (*execFun)(),
                             bool recalcRepTasks = true, bool isBlocking = false);

    /**
     * Add new task executing addTask specific time
     * @param hour Hour when task must be executed.
     * @param minute Minute when task must be executed.
     * @param second Second when task must be executed.
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param recalcRepTasks (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time.
     * When true - all outdated tasks will be skipped, if false - execution time of all task will be recalculated, after end this task.
     * @param isBlocking if true all tasks run on thread must end before start this task and all another tasks will wait until this task end.
     */
    void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                             TaskClassInterface *clss, bool recalcRepTasks = true, bool isBlocking = false);

    //TODO make constexpr

    /**
     * Set after how many minutes sheduler must end work. If not set sheduler never ending.
     * @param time Time from begin to end sheduler work. It schould be chrono time value (eg. chrono::minutes)
     */
    void setEndWorkTimeAfter(chrono::microseconds time);

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
     * @param time in chrono time value (eg. chrono::minutes) default 2h
     */
    void setMaxTimeGap(chrono::microseconds time);

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
     * @param delayBetweenTasks time in chrono rime value (eg. chrono::microseconds)
     */
    void setDelayBetweenTasks(const chrono::microseconds &delayBetweenTasks);

    /**
     * Set accepted delay in calling tasks. If delay is exceeded time of next call will be racalculate
     * @param acceptedDelay time in chrono rime value (eg. chrono::microseconds) (default 1s)
     */
    void setAcceptedDelay(const chrono::microseconds &acceptedDelay);

    /**
     * @brief Run sheduler, start executing tasks.
     */
    void run();

    //TODO stop

private:
    struct TaskListItem {
        TaskRepeatable *repetableTask;
        TaskStaticTime *staticTask;
        system_clock_time execTime;

        TaskListItem(unique_ptr<TaskRepeatable> &repeatable,
                     const system_clock_time &execTime): repetableTask(repeatable.get()), staticTask(nullptr),
                                                         execTime(execTime) {
        }

        TaskListItem(unique_ptr<TaskStaticTime> &staticTask, const system_clock_time &execTime): repetableTask(nullptr),
            staticTask(staticTask.get()), execTime(execTime) {
        }
    };

    const unsigned short planedTasks = 4;
    system_clock_time endWorkingTime, now;
    chrono::microseconds delayBetweenTasks = 1s;
    chrono::microseconds acceptedDelay = 1s;
    chrono::microseconds maxTimeGap = 2h;
    vector<unique_ptr<TaskRepeatable> > repeatableTaskList;
    vector<unique_ptr<TaskStaticTime> > staticTimeTaskList;

    deque<TaskListItem> taskQueue;
    map<unsigned int, future<void> > mapTasksOnThread;

    unsigned int taskId = 0;
    bool flgEndWorkTimeEnabled = false;
    bool flgEndWhenRepeatableEnd = true;


    bool checkCorrectTime(unsigned short hour, unsigned short minute, unsigned short second);

    bool checkCorrectDate(unsigned short year, unsigned short month, unsigned short day);

    time_t calculateEndTime(time_t base, unsigned short hour, unsigned short minute, unsigned short second);

    void prepareRun();

    void preapreRunRepeatableTasks();

    void runLoop();

    bool mainLoopCondition();

    void waitToEndAllTasksOnThread();

    void updateTasksOnThread();

    void executeStaticTimeTask(deque<TaskListItem>::iterator &actualtaskContr);

    void executeRepeatabletask(deque<TaskListItem>::iterator &actualtaskContr);

    void repeatRepatableTask(deque<TaskListItem>::iterator &actualtaskContr);

    chrono::microseconds calcSleepTime();

    system_clock_time getTimeOfLastElement(const TaskListItem &task) const;

    void processDelays();

    void updateNow();

    void addTaskToQueue(const TaskListItem &task);
};

#endif // SCHEDULER_H
