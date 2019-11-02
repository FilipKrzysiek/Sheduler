#ifndef SHEDULERLIST_H
#include <string>
using namespace std;
#define SHEDULERLIST_H

class ShedulerList {
public:
    ShedulerList();
    ShedulerList(unsigned int planedExec);
    virtual ~ShedulerList();

    bool next();
    Task *getTask();
    time_t getExecTime();
    time_t getNextExecTime();
    bool nextIsTheSame();
    bool nextIsExisting();
    bool isExisting();
    void setAmountPlanedExec(unsigned int ne);
    void add(Task *tsk, time_t timeExec);

    void printTasksId();
    string getTaskTimeList();
    void addActual();
protected:

private:
    class waitTask {
    public:
        Task *task;
        time_t timeExec;
        waitTask *next = nullptr;

        waitTask(Task *task, time_t timeExec, waitTask *next = nullptr) {
            this->task = task;
            this->timeExec = timeExec;
            this->next = next;
        }
    };

    waitTask *wTaskActual = nullptr;
    bool nextExisting;
    unsigned int planedExec;
};

#endif // SHEDULERLIST_H
