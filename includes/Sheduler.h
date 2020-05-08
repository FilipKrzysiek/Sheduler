#ifndef SHEDULER_H

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "TaskClassInterface.h"
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
     * @brief Run sheduler, start executing tasks.
     */
    void run();

private:
    //time_t endWorkingTime, now, slept, maxTimeGap = 1000 * 60 * 10;
    chrono::time_point<chrono::system_clock> endWorkingTime, now;
    chrono::milliseconds slept;
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
    void prepareRun(chrono::milliseconds delayBetweenTasks);

    void runLoop();

    void executeTask();

    chrono::milliseconds calcSleepTime(chrono::time_point<chrono::system_clock> execTime);

    string getTaskTimeList();

    chrono::milliseconds getSleept();
};

#endif // SHEDULER_H
