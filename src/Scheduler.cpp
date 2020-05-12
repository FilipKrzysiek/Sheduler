#include "../includes/Scheduler.h"
#include "../includes/TaskRepeatable.h"

using namespace std;

//TODO task deterministic
//TODO task adaptative (calc time from end last task)
//TODO task on specific time expropriate (other tasks in the same time was canceled)
//TODO task on specific time not expropriated (other task is delayed)

Scheduler::Scheduler() {
    this->taskId = 0;
    this->planedExec = 3;
}

Scheduler::Scheduler(chrono::minutes endAfterMinutes) {
    this->taskId = 0;
    this->planedExec = 3;
    setEndWorkTimeAfter(endAfterMinutes);
}

Scheduler::~Scheduler() {
    for (auto tsk : repeatableTaskList) {
        delete tsk;
    }

    for (auto tsk : staticTimeTaskList) {
        delete tsk;
    }
}

void Scheduler::addNewTask(chrono::seconds interval, void (*execFun)(), bool canSkipped, chrono::seconds endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    repeatableTaskList.push_back(new TaskFunction(taskId, interval, execFun, canSkipped, endAfter));
    taskId++;
}

void Scheduler::addNewTask(chrono::seconds interval, TaskClassInterface *clss, bool canSkipped,
                           chrono::seconds endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    repeatableTaskList.push_back(new TaskClass(taskId, interval, clss, canSkipped, endAfter));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(),
                                    bool skippOtherTasks) {
    staticTimeTaskList.push_back(
            new TaskFunction(taskId, calculateExecuteTime(timeToExecute), execFun, skippOtherTasks));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool skippOtherTasks) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), execFun, skippOtherTasks);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    void (*execFun)(), bool skippOtherTasks) {
    if (!checkCorrectTime(hour, minute, second))
        throw "Passed time to create task AT is incorrect";

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, execFun, skippOtherTasks);
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss,
                                    bool skippOtherTasks) {
    staticTimeTaskList.push_back(new TaskClass(taskId, calculateExecuteTime(timeToExecute), clss, skippOtherTasks));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool skippOtherTasks) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), clss, skippOtherTasks);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    TaskClassInterface *clss, bool skippOtherTasks) {
    if (!checkCorrectTime(hour, minute, second))
        throw "Passed time to create task AT is incorrect";

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, clss, skippOtherTasks);
}

void Scheduler::setEndWorkTimeAfter(chrono::minutes minutes) {
    this->endWorkingTime = chrono::system_clock::now() + minutes;
    flgEndWorkTimeEnabled = true;
}

void Scheduler::setEndWorkTime(system_clock_time date, bool getOnlyTime) {
    if (date < chrono::system_clock::now()) {
        if (getOnlyTime) {
            date = date + 24h;
        } else
            throw "Set end time before start time";
    }

    endWorkingTime = date;
    flgEndWorkTimeEnabled = true;
}

void Scheduler::setMaxTimeGap(chrono::minutes minutes) {
    if (minutes > 70min)
        throw "Trying set to big max time gap. Max value is 70minutes";

    maxTimeGap = minutes;
}

string Scheduler::getTaskTimeList() {
    return schedulerQueueRepeatable.getTaskTimeList();
}

chrono::milliseconds Scheduler::getSleept() {
    return slept;
}

bool Scheduler::checkCorrectTime(unsigned short hour, unsigned short minute, unsigned short second) {
    return !(hour > 23 || minute > 59 || second > 59);
}

bool Scheduler::checkCorrectDate(unsigned short year, unsigned short month, unsigned short day) {
    if (month > 12 || month == 0 || day == 0)
        return false;

    if (month == 2) {
        if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) {
            if (day > 29)
                return false;
        } else {
            if (day > 28)
                return false;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    } else {
        if (day > 31)
            return false;
    }

    return true;
}

time_t Scheduler::calculateEndTime(time_t base, unsigned short hour, unsigned short minute, unsigned short second) {
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

system_clock_time Scheduler::calculateExecuteTime(system_clock_time executeTime) {
    time_t timeTNow, timeTExecuteTime;
    time(&timeTNow);
    timeTExecuteTime = chrono::system_clock::to_time_t(executeTime);
    tm *localNow, *localExec;
    localNow = localtime(&timeTNow);
    localExec = localtime(&timeTExecuteTime);

    localNow->tm_hour = localExec->tm_hour;
    localNow->tm_min = localExec->tm_min;
    localNow->tm_sec = localExec->tm_sec;

    timeTExecuteTime = mktime(localNow);

    return chrono::system_clock::from_time_t(timeTExecuteTime);
}

void Scheduler::setEndWorkTime(time_t date, bool getOnlyTime) {
    setEndWorkTime(chrono::system_clock::from_time_t(date), getOnlyTime);
}

void Scheduler::setEndWorkTime(unsigned short hour, unsigned short minute, unsigned short second, unsigned short year,
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
        if (!checkCorrectTime(hour, minute, second))
            throw "Passed time to setEndWorkTime is incorrect";

        if (!checkCorrectDate(year, month, day))
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
        if (nowTimeT > newTimeT)
            throw "Passed time to setEndWorkingTime is in the past";

        setEndWorkTime(newTimeT, false);
    }

}

void Scheduler::setEndWhenRepeatableEnd(bool flg) {
    flgEndWhenRepeatableEnd = flg;
}

void Scheduler::setDelayBetweenTasks(const chrono::milliseconds &delayBetweenTasks) {
    Scheduler::delayBetweenTasks = delayBetweenTasks;
}




//-------------------------------------

void Scheduler::run() {
    this->prepareRun();
    if (!flgEndWorkTimeEnabled) {
        while (1) {
            if ((!schedulerQueueRepeatable.isExisting() && flgEndWhenRepeatableEnd) ||
                (!schedulerQueueRepeatable.isExisting() && !schedulerQueueStaticTime.isExisting())) {
                break;
            } else {
                runLoop();
            }
        }
    } else {
        cout << chrono::system_clock::to_time_t(now) << " - " << chrono::system_clock::to_time_t(endWorkingTime)
             << endl;
        while (now < this->endWorkingTime) {
            //this->schedulerQueueRepeatable.printTasksId();
            if ((!schedulerQueueRepeatable.isExisting() && flgEndWhenRepeatableEnd) ||
                (!schedulerQueueRepeatable.isExisting() && !schedulerQueueStaticTime.isExisting())) {
                break;
            } else {
                runLoop();
            }
        }
    }
}

void Scheduler::prepareRun() {
    if (flgEndWhenRepeatableEnd) {
        if (repeatableTaskList.empty())
            throw "Repeatable list is empty. Set flg endWhenRepeatableEnd = true";
    } else {
        if (repeatableTaskList.empty() && staticTimeTaskList.empty())
            throw "Task lists is empty. No added tasks to list!";
    }

    prepareRunRepeatable();
    prepareRunStaticTime();
}

void Scheduler::prepareRunRepeatable() {
    schedulerQueueRepeatable.setAmountPlanedExec(planedExec);
    now = std::chrono::system_clock::now();

    Task *tsk;
    for (int i = 0; i < repeatableTaskList.size(); i++) {
        tsk = repeatableTaskList.at(i);
        for (int j = 0; j < planedExec; j++) {
            system_clock_time timeTo = j * tsk->getInterval() + now + delayBetweenTasks * i;
            if (tsk->getIsNeverEnding() || tsk->getEndTime() > timeTo)
                schedulerQueueRepeatable.addTask(tsk, timeTo);
        }
    }
}

void Scheduler::prepareRunStaticTime() {
    schedulerQueueStaticTime.setAmountPlanedExec(1);

    for (Task *tsk : staticTimeTaskList) {
        if (now > tsk->getStaticExecuteTime())
            tsk->incrementStaticExecuteTime();

        schedulerQueueStaticTime.addTask(tsk, tsk->getStaticExecuteTime());
    }
}

void Scheduler::runLoop() {
    now = std::chrono::system_clock::now();
    if (!runStaticTime()) {
        runRepeatable();
    }
    slept = calcSleepTime();
    this_thread::sleep_for(slept);
}

bool Scheduler::runStaticTime() {
    if (now >= schedulerQueueStaticTime.getExecTime()) {
        Task *tsk = schedulerQueueStaticTime.getTask();
        tsk->incrementStaticExecuteTime();
        schedulerQueueStaticTime.addTask(tsk, schedulerQueueStaticTime.getExecTime());

        tsk->execute();

        if (tsk->getSkipOtherTasks()) {
            schedulerQueueRepeatable.clearQueue();
            prepareRunRepeatable();
        }
        schedulerQueueStaticTime.next();
        return true;
    }
    return false;
}

void Scheduler::runRepeatable() {
    schedulerQueueRepeatable.addActual();

    if (now - schedulerQueueRepeatable.getExecTime() > -1s) {
        if (schedulerQueueRepeatable.nextIsExisting()) {
            if (now - schedulerQueueRepeatable.getExecTime() < 1s) {
                executeRepeatableTask();
            } else {
                if (schedulerQueueRepeatable.getTask()->getCanSkipped()) {
                    skippingTasks();
                } else {
                    executeRepeatableTask();
                }
            }
        } else {
            executeRepeatableTask();
        }
    }
}

// Skiping if next is the same (change pointer in queue)
// Skiping if next execute of this task have time lower than now
void Scheduler::skippingTasks() {
    if (schedulerQueueRepeatable.nextIsTheSame()) {
        schedulerQueueRepeatable.next();
    } else {
        if (schedulerQueueRepeatable.nextThisTaskIsExisting()) {
            if (now - schedulerQueueRepeatable.getNextThisTaskExecTime() < 0s) {
                executeRepeatableTask();
            } else {
                schedulerQueueRepeatable.next();
            }
        } else {
            executeRepeatableTask();
        }
    }
}

void Scheduler::executeRepeatableTask() {
    this->schedulerQueueRepeatable.getTask()->execute();
    this->schedulerQueueRepeatable.next();
}

chrono::milliseconds Scheduler::calcSleepTime() {
    system_clock_time execTime;
    if (schedulerQueueRepeatable.getExecTime() >= schedulerQueueStaticTime.getExecTime()) {
        execTime = schedulerQueueStaticTime.getExecTime();
    } else {
        execTime = schedulerQueueRepeatable.getExecTime();
    }
    chrono::milliseconds sleepTime = chrono::duration_cast<chrono::milliseconds>(execTime - now);
    if (sleepTime > maxTimeGap || sleepTime < 0s) {
        throw "Error calculed time between tasks was bigger than set max time gap";
    }

    return sleepTime;
}


