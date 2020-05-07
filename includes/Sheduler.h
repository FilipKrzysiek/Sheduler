#ifndef SHEDULER_H

//#ifdef _WIN32
//#define _HAS_STD_BYTE 0
//#include <windows.h>
//void sleep(unsigned milliseconds)
//{
//    Sleep(milliseconds);
//}
//#else
//
//#include <unistd.h>
//
//unsigned sleep(unsigned int milliseconds) {
//    usleep(milliseconds * 1000); // takes microseconds
//}
//
//#endif

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

    Sheduler(chrono::minutes endAfterMinutes);

    virtual ~Sheduler();

    //TODO If no passed endAfter program not start, end all correctly. Not error, but not run any tasks.
    //TODO endAfter not working, passed tasks work over end time
    //TODO Exceptions as exception, not as const char*
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

    void run();

    string getTaskTimeList();

    chrono::milliseconds getSleept();

protected:

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
};

#endif // SHEDULER_H
