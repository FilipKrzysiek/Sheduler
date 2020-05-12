#include "../includes/Sheduler.h"
#include "../includes/TaskRepeatable.h"

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
    this->taskList.push_back(new TaskFunction<TaskRepeatable>(this->taskId, interval, execFun, canSkipped, endAfter));

    this->taskId++;
}

void
Sheduler::addNewTask(chrono::seconds interval, TaskClassInterface *clss, bool canSkipped, chrono::seconds endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    this->taskList.push_back(new TaskClass<TaskRepeatable>(this->taskId, interval, clss, canSkipped, endAfter));

    this->taskId++;
}

void Sheduler::setEndWorkTimeAfter(chrono::minutes minutes) {
    this->endWorkingTime = chrono::system_clock::now() + minutes;
    flgEndWorkTimeEnabled = true;
}

void Sheduler::setEndWorkTime(chrono::time_point<chrono::system_clock> date, bool getOnlyTime) {
    if (date < chrono::system_clock::now()) {
        if(getOnlyTime){
            date = date + 24h;
        } else
            throw "Set end time before start time";
    }

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
        throw "Error calculed time between tasks was bigger than set max time gap";
    }

    return sleeptime;
}


string Sheduler::getTaskTimeList() {
    return shList.getTaskTimeList();
}

chrono::milliseconds Sheduler::getSleept() {
    return slept;
}

bool Sheduler::checkCorrectTime(unsigned short hour, unsigned short minute, unsigned short second) {
    return !(hour > 23 || minute > 59 || second > 59);
}

bool Sheduler::checkCorrectDate(unsigned short year, unsigned short month, unsigned short day) {
    if(month > 12 || month == 0 || day == 0)
        return false;

    if(month == 2){
        if(year%400 == 0 || (year%100 != 0 && year%4 == 0)){
            if(day > 29)
                return false;
        } else {
            if(day > 28)
                return false;
        }
    }
    else if(month == 4 || month == 6 || month == 9 || month == 11 ){
        if(day > 30)
            return false;
    } else {
        if(day > 31)
            return false;
    }

    return true;
}

time_t Sheduler::calculateEndTime(time_t base, unsigned short hour, unsigned short minute, unsigned short second) {
    tm *localTime = localtime(&base);
    if (localTime->tm_hour > hour) {
        base += 24 * 60 * 60;
        localTime = localtime(&base);
    }

    localTime->tm_hour = hour;
    localTime->tm_min = minute;
    localTime->tm_sec = second;

    return mktime(localTime);
}

void Sheduler::addNewTaskCallingAt(chrono::time_point<chrono::system_clock> timeToExecute, void (*execFun)(),
                                   bool skippOtherTasks) {

}

void Sheduler::addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool skippOtherTasks) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), execFun, skippOtherTasks);
}

void Sheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second, void (*execFun)(),
                                   bool skippOtherTasks) {
    if (!checkCorrectTime(hour, minute, second))
        throw "Passed time to create task AT is incorrect";

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, execFun, skippOtherTasks);
}

void Sheduler::addNewTaskCallingAt(chrono::time_point<chrono::system_clock> timeToExecute, TaskClassInterface *clss,
                                   bool skippOtherTasks) {

}

void Sheduler::addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool skippOtherTasks) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), clss, skippOtherTasks);
}

void Sheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                   TaskClassInterface *clss, bool skippOtherTasks) {
    if (!checkCorrectTime(hour, minute, second))
        throw "Passed time to create task AT is incorrect";

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, clss, skippOtherTasks);
}

void Sheduler::setEndWorkTime(time_t date, bool getOnlyTime) {
    setEndWorkTime(chrono::system_clock::from_time_t(date), getOnlyTime);
}

void Sheduler::setEndWorktime(unsigned short hour, unsigned short minute, unsigned short second, unsigned short year,
                              unsigned short month, unsigned short day) {
    if (65535 == hour || 65535 == minute || 65535 == second) {
        if (65535 == hour == minute == second) {
            time_t nowTimeT;
            time(&nowTimeT);
            nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
            setEndWorkTime(nowTimeT, true);
        } else {
            throw "At setEndWorkingTime passed not all date. Maybe missing day or month and day.";
        }
    } else {
        if(!checkCorrectTime(hour, minute, second))
            throw "Passed time to setEndWorkTime is incorrect";

        if(!checkCorrectDate(year, month, day))
            throw "Passed date to setEndWorkingTime is incorrect";

        time_t newTimeT;
        time(&newTimeT);
        tm *lTime = localtime(&newTimeT);
        lTime->tm_year = year - 1900;
        lTime->tm_mon = month - 1;
        lTime->tm_mday = day;
        lTime->tm_hour = hour;
        lTime->tm_min = minute;
        lTime->tm_sec = second;

        newTimeT = mktime(lTime);

        time_t nowTimeT;
        time(&nowTimeT);
        if(nowTimeT > newTimeT)
            throw "Passed time to setEndWorkingTime is in the past";

        setEndWorkTime(newTimeT, false);
    }

}
