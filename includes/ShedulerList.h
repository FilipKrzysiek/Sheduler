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

#endif // SHEDULERLIST_H
