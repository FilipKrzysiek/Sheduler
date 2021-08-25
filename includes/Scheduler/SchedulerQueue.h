#ifndef SHEDULERLIST_H

#include <iostream>
#include <string>
#include "Task.h"

using namespace std;
#define SHEDULERLIST_H

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

#endif // SHEDULERLIST_H
