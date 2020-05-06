#include "../includes/Sheduler.h"

using namespace std;

//TODO task deterministic
//TODO task adaptative (calc time from end last task)
//TODO task on specific time expropriate (other tasks in the same time was canceled)
//TODO task on specific time not expropriated (other task is delayed)
//TODO time_t to chrono

Sheduler::Sheduler() {
    this->taskId = 0;
    this->planedExec = 3;
}

Sheduler::Sheduler(unsigned int endAfterMinutes) {
    this->taskId = 0;
    this->planedExec = 3;
    setEndWorkTime(endAfterMinutes);
}

Sheduler::~Sheduler() {
    //dtor
}

void Sheduler::addNewTask(unsigned int interval, void (*execFun)(), bool canSkipped, unsigned int endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    this->taskList.push_back(new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter));

    this->taskId++;
}

void Sheduler::addNewTask(unsigned int interval, TaskClassInterface *clss, bool canSkipped, unsigned int endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    this->taskList.push_back(new TaskClassIf(this->taskId, interval, clss, canSkipped, endAfter));

    this->taskId++;
}

void Sheduler::setEndWorkTime(unsigned int minutes) {
    time(&this->now);
    this->endWorkingTime = now + minutes * 60;
}

void Sheduler::setMaxTimeGap(unsigned short minutes) {
    if (minutes > 70)
        throw "Trying set to big max time gap. Max value is 70minutes";

    maxTimeGap = minutes * 1000 * 60;
}

void Sheduler::run() {
    this->prepareRun(1);
    if (this->endWorkingTime == 0) {
        while (1) {
            if (this->shList.isExisting()) {
                this->runLoop();
            } else {
                break;
            }
        }
    } else {
        cout << now << " - " << endWorkingTime << endl;
        while (now < this->endWorkingTime) {
            //this->shList.printTasksId();
            if (this->shList.isExisting()) {
                this->runLoop();
                time(&now);
            } else {
                //End work when nothing to work
                break;
            }
        }
    }
}

void Sheduler::prepareRun(unsigned int delayBetweenTasks = 0) {
    this->shList.setAmountPlanedExec(this->planedExec);
    time(&this->now);
    Task *tsk;
    //Checking empty task list
    if (this->taskList.empty())
        throw "Task list is empty";

    for (int i = 0; i < this->taskList.size(); i++) {
        tsk = this->taskList.at(i);
        for (int j = 0; j < this->planedExec; j++) {
            time_t timeTo = j * tsk->getInterval() + now + delayBetweenTasks * i;
            if (tsk->getEndTime() > timeTo)
                this->shList.add(tsk, timeTo);
        }
    }
}

void Sheduler::runLoop() {
    time_t thisTime;
    thisTime = this->shList.getExecTime();
    //There no destructing interval, but nie uwzględnia delays.

    time(&now);
    this->shList.addActual();

    if (this->shList.nextIsExisting()) {
        //Check is delay act on next task
        if (this->shList.getNextExecTime() <= now) {
            //Checking skipping is allowed
            if (this->shList.getTask()->getCanSkipped()) {
                //Check can be skipp
                if (this->shList.nextIsTheSame()) {
                    this->shList.next();
                } else {
                    this->executeTask();
                }
            } else {
                this->executeTask();
            }
            //Check delay actual task
        } else if (thisTime <= now) {
            this->executeTask();
        } else {
            sleept = calcSleepTime(thisTime);
            sleep(sleept);
            this->executeTask();
        }
    } else {
        //Execute last job
        if (thisTime > now) {
            sleept = calcSleepTime(thisTime);
            sleep(sleept);
        }
        this->executeTask();
    }
}

void Sheduler::executeTask() {
    Task *tsk = this->shList.getTask();
    tsk->execute();
    this->shList.next();
}

time_t Sheduler::calcSleepTime(time_t execTime) {
    execTime = execTime - now;
    if (execTime > maxTimeGap || execTime < 0) {
        throw "Error calculed time between tasks was bigger than setted max time gap";
    }

    return execTime * 1000;
}


string Sheduler::getTaskTimeList() {
    return shList.getTaskTimeList();
}

time_t Sheduler::getSleept() {
    return sleept;
}
