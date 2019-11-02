#include <vector>
#include <string>
#include <iostream>

using namespace std;

class TaskClassInterface{
public:
    /**
     * @brief Execute class method
     */
    virtual void execute() = 0;
};

class Task
{
public:
    //Task(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped, unsigned int endAfter){};
    //virtual Task(unsigned int interval, string execString, bool canSkipped = true, unsigned int endAfter = 0);
    Task(){};
    virtual ~Task(){};
    virtual void execute() = 0;

    //Getters
    time_t getLastExecute();
    bool getCanSkipped();

    /**
     * @brief Check pass end work time
     * @return
     */
    bool outOfTime();
    time_t getEndTime();
    unsigned int getId();
    unsigned int getInterval();

};

class TaskClassIf: public Task{
public:
    TaskClassIf(unsigned int id, unsigned int interval, TaskClassInterface *clss, bool canSkipped = true, unsigned int endAfter = 0);
    virtual ~TaskClassIf(){};
    void execute() override ;
};

class TaskFunction : public Task
{
public:
    TaskFunction(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped = true, unsigned int endAfter = 0);
    virtual ~TaskFunction(){};
    void execute() override ;
};

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
};

