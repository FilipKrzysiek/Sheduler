#ifndef SHEDULER_H

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "TaskClassInterface.h"
#include "TaskRepeatable.h"
#include "../src/Task.cpp"
#include "../src/TaskFunction.cpp"
#include "../src/TaskClass.cpp"
#include "../src/ShedulerList.cpp"

using namespace std;
using namespace std::chrono_literals;
#define SHEDULER_H


class Sheduler {
public:
    Sheduler();

    /**
     * Create sheduler and set time after app wnd work.
     * @param endAfterMinutes
     */
    Sheduler(chrono::minutes endAfterMinutes);

    virtual ~Sheduler();

    //TODO Overwrite method setEndWorkTime to pass year, mont, day, hour, minute, second and hour, minute, second
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
     * Add new task executing add specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(chrono::time_point<chrono::system_clock> timeToExecute, void (*execFun)(),
                             bool skippOtherTasks = true);

    /**
     * Add new task executing add specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(chrono::time_point<chrono::system_clock> timeToExecute, TaskClassInterface *clss,
                             bool skippOtherTasks = true);

    /**
     * Add new task executing add specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param execFun Address for function to call (execute).
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool skippOtherTasks = true);

    /**
     * Add new task executing add specific time
     * @param timeToExecute Time when task must be executed (from passed date only was used hour, minute, and second)
     * @param clss Address to class which was expanded by TaskClassInterface where was method to calling.
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool skippOtherTasks = true);

    /**
     * Add new task executing add specific time
     * @param hour Hour when task must be executed.
     * @param minute Minute when task must be executed.
     * @param second Second when task must be executed.
     * @param execFun Address for function to call (execute).
     * @param skippOtherTasks Skipping other periodic task, when this task was executing (checked can't skipped too) default true
     */
    void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second, void (*execFun)(),
                             bool skippOtherTasks = true);

    /**
     * Add new task executing add specific time
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
    void setEndWorkTime(chrono::time_point<chrono::system_clock> date, bool getOnlyTime = true);

    /**
     * Set datetime when scheduler must end work. If not set scheduler never ending.
     * @param hour Hour at scheduler must end work.
     * @param minute Minute of passed hour, when scheduler must end work.
     * @param second Second of passed minute, when scheduler must end work.
     * @param year Year when scheduler must end work (if not passed get this year).
     * @param month Month of passed year, when scheduler must end work (if not passed get this month).
     * @param day Day of passed month, when scheduler must end work (if not passed get this day).
     */
    void setEndWorktime(unsigned short hour, unsigned short minute, unsigned short second,
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
     * @brief Run sheduler, start executing tasks.
     */
    void run();

private:
    //time_t endWorkingTime, now, slept, maxTimeGap = 1000 * 60 * 10;
    chrono::time_point<chrono::system_clock> endWorkingTime, now;
    chrono::milliseconds slept;
    chrono::seconds maxTimeGap = 10min;
    vector<Task *> taskList;
    ShedulerList shList;
    unsigned int taskId;
    unsigned int planedExec;
    bool flgEndWorkTimeEnabled = false;

    /**
     *
     * @param delayBetweenTasks - seconds betwen first runs tasks
     */
    void prepareRun(chrono::milliseconds delayBetweenTasks);

    void runLoop();

    void executeTask();

    bool checkCorrectTime(unsigned short hour, unsigned short minute, unsigned short second);

    bool checkCorrectDate(unsigned short year, unsigned short month, unsigned short day);

    time_t calculateEndTime(time_t base, unsigned short hour, unsigned short minute, unsigned short second);

    chrono::milliseconds calcSleepTime(chrono::time_point<chrono::system_clock> execTime);

    string getTaskTimeList();

    chrono::milliseconds getSleept();
};

#endif // SHEDULER_H
