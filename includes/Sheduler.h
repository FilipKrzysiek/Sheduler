#ifndef SHEDULER_H

#ifdef _WIN32
#define _HAS_STD_BYTE 0
#include <windows.h>
void sleep(unsigned milliseconds)
{
    Sleep(milliseconds);
}
#else

#include <unistd.h>
unsigned sleep(unsigned int milliseconds) {
    usleep(milliseconds * 1000); // takes microseconds
}

#endif

#include <vector>
#include <string>
#include <iostream>
#include "TaskClassInterface.h"
#include "../src/Task.cpp"
#include "../src/TaskFunction.cpp"
#include "../src/TaskClassIf.cpp"
#include "../src/ShedulerList.cpp"

using namespace std;
#define SHEDULER_H


class Sheduler {
public:
    Sheduler();

    Sheduler(unsigned int endAfterMinutes);

    virtual ~Sheduler();

    void addNewTask(unsigned int interval, void (*execFun)(), bool canSkipped = true, unsigned int endAfter = 0);

    void addNewTask(unsigned int interval, TaskClassInterface *clss, bool canSkipped = true, unsigned int endAfter = 0);

    void setEndWorkTime(unsigned int minutes);

    /**
     * @brief Set max time gap between tasks. If time gap is higher than declared throw error.
     * @param minutes max value 70
     */
    void setMaxTimeGap(unsigned short minutes);

    void run();

    string getTaskTimeList();

    time_t getSleept();

protected:

private:
    time_t endWorkingTime, now, sleept, maxTimeGap = 1000 * 60 * 10;
    vector<Task *> taskList;
    ShedulerList shList;
    unsigned int taskId;
    unsigned int planedExec;

    void prepareRun(unsigned int delayBetweenTasks);

    void runLoop();

    void executeTask();

    time_t calcSleepTime(time_t execTime);
};

#endif // SHEDULER_H
