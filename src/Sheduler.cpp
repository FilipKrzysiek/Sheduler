#include "../includes/Sheduler.h"

using namespace std;

//TODO task deterministic
//TODO task adaptative (calc time from end last task)
//TODO task on specific time expropriate (other tasks in the same time was canceled)
//TODO task on specific time not expropriated (other task is delayed)

Sheduler::Sheduler() {
    this->taskId = 0;
    this->planedExec = 3;
}

Sheduler::Sheduler(chrono::minutes endAfterMinutes) {
    this->taskId = 0;
    this->planedExec = 3;
    setEndWorkTimeAfter(endAfterMinutes);
}

Sheduler::~Sheduler() {
    //dtor
}

void Sheduler::addNewTask(chrono::seconds interval, void (*execFun)(), bool canSkipped, chrono::seconds endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    this->taskList.push_back(new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter));

    this->taskId++;
}

void
Sheduler::addNewTask(chrono::seconds interval, TaskClassInterface *clss, bool canSkipped, chrono::seconds endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    this->taskList.push_back(new TaskClass(this->taskId, interval, clss, canSkipped, endAfter));

    this->taskId++;
}

void Sheduler::setEndWorkTimeAfter(chrono::minutes minutes) {
    this->endWorkingTime = chrono::system_clock::now() + minutes;
    flgEndWorkTimeEnabled = true;
}

void Sheduler::setEndWorkTime(chrono::time_point<chrono::system_clock> date, bool getOnlyTime) {
    if (date < chrono::system_clock::now())
        throw "Setted end time before start time";

    endWorkingTime = date;
    flgEndWorkTimeEnabled = true;
}

void Sheduler::setMaxTimeGap(chrono::minutes minutes) {
    if (minutes > 70min)
        throw "Trying set to big max time gap. Max value is 70minutes";

    maxTimeGap = minutes;
}

void Sheduler::run() {
    this->prepareRun(1s);
    if (!flgEndWorkTimeEnabled) {
        while (1) {
            if (this->shList.isExisting()) {
                this->runLoop();
            } else {
                break;
            }
        }
    } else {
        cout << chrono::system_clock::to_time_t(now) << " - " << chrono::system_clock::to_time_t(endWorkingTime)
             << endl;
        while (now < this->endWorkingTime) {
            //this->shList.printTasksId();
            if (this->shList.isExisting()) {
                this->runLoop();
                now = std::chrono::system_clock::now();
            } else {
                //End work when nothing to work
                break;
            }
        }
    }
}

void Sheduler::prepareRun(chrono::milliseconds delayBetweenTasks = 0ms) {
    this->shList.setAmountPlanedExec(this->planedExec);
    now = std::chrono::system_clock::now();
    Task *tsk;
    //Checking empty task list
    if (this->taskList.empty())
        throw "Task list is empty";

    for (int i = 0; i < this->taskList.size(); i++) {
        tsk = this->taskList.at(i);
        for (int j = 0; j < this->planedExec; j++) {
            chrono::time_point<chrono::system_clock> timeTo = j * tsk->getInterval() + now + delayBetweenTasks * i;
            if (tsk->getIsNeverEnding() || tsk->getEndTime() > timeTo)
                this->shList.add(tsk, timeTo);
        }
    }
}

void Sheduler::runLoop() {
    chrono::time_point<chrono::system_clock> thisTime;
    thisTime = this->shList.getExecTime();
    //There no destructing interval, but nie uwzględnia delays.

    now = std::chrono::system_clock::now();
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
            slept = calcSleepTime(thisTime);
            this_thread::sleep_for(slept);
            this->executeTask();
        }
    } else {
        //Execute last job
        if (thisTime > now) {
            slept = calcSleepTime(thisTime);
            this_thread::sleep_for(slept);
        }
        this->executeTask();
    }
}

void Sheduler::executeTask() {
    Task *tsk = this->shList.getTask();
    tsk->execute();
    this->shList.next();
}

chrono::milliseconds Sheduler::calcSleepTime(chrono::time_point<chrono::system_clock> execTime) {
    chrono::milliseconds sleeptime = chrono::duration_cast<chrono::milliseconds>(execTime - now);
    if (sleeptime > maxTimeGap || sleeptime < 0s) {
        throw "Error calculed time between tasks was bigger than setted max time gap";
    }

    return sleeptime;
}


string Sheduler::getTaskTimeList() {
    return shList.getTaskTimeList();
}

chrono::milliseconds Sheduler::getSleept() {
    return slept;
}
