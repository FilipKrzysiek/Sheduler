#include "../includes/Scheduler.h"


using namespace std;

//TODO delete ended task from list
//TODO task adaptative (calc time from end last task)

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
    flgEndTaskCollector = true;
    if (taskCollector.joinable())
        taskCollector.join();
    for (auto tsk : repeatableTaskList) {
        delete tsk;
    }

    for (auto tsk : staticTimeTaskList) {
        delete tsk;
    }
}

void Scheduler::addNewTask(chrono::seconds interval, void (*execFun)(), bool canSkipped, bool isBlocking,
                           chrono::seconds endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    repeatableTaskList.push_back(new TaskFunction(taskId, interval, execFun, canSkipped, isBlocking, false, endAfter));
    taskId++;
}

void Scheduler::addNewTask(chrono::seconds interval, TaskClassInterface *clss, bool canSkipped, bool isBlocking,
                           chrono::seconds endAfter) {
    //Task *newTask = new TaskFunction(this->taskId, interval, execFun, canSkipped, endAfter);
    repeatableTaskList.push_back(new TaskClass(taskId, interval, clss, canSkipped, isBlocking, false, endAfter));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(), bool skippOtherTasks,
                                    bool isBlocking) {
    staticTimeTaskList.push_back(new TaskFunction(taskId, calculateExecuteTime(timeToExecute), execFun,
                                                  skippOtherTasks, isBlocking, false));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool skippOtherTasks, bool isBlocking) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), execFun, skippOtherTasks, isBlocking);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    void (*execFun)(), bool skippOtherTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw Exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, execFun, skippOtherTasks, isBlocking);
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss, bool skippOtherTasks,
                                    bool isBlocking) {
    staticTimeTaskList.push_back(new TaskClass(taskId, calculateExecuteTime(timeToExecute), clss,
                                               skippOtherTasks, isBlocking, false));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool skippOtherTasks,
                                    bool isBlocking) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), clss, skippOtherTasks, isBlocking);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    TaskClassInterface *clss, bool skippOtherTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw Exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, clss, skippOtherTasks, isBlocking);
}


void Scheduler::addNewTaskOnThread(chrono::seconds interval, void (*execFun)(), bool canSkipped, bool isBlocking,
                                   chrono::seconds endAfter) {
    repeatableTaskList.push_back(new TaskFunction(taskId, interval, execFun, canSkipped, isBlocking, true, endAfter));
    taskId++;
}

void Scheduler::addNewTaskOnThead(chrono::seconds interval, TaskClassInterface *clss, bool canSkipped, bool isBlocking,
                                  chrono::seconds endAfter) {
    repeatableTaskList.push_back(new TaskClass(taskId, interval, clss, canSkipped, isBlocking, true, endAfter));
    taskId++;
}

void Scheduler::addNewTaskOnThreadCallingAt(system_clock_time timeToExecute, void (*execFun)(), bool skippOtherTasks,
                                            bool isBlocking) {
    staticTimeTaskList.push_back(new TaskFunction(taskId, calculateExecuteTime(timeToExecute), execFun,
                                                  skippOtherTasks, isBlocking, true));
    taskId++;
}

void
Scheduler::addNewTaskOnThreadCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss, bool skippOtherTasks,
                                       bool isBlocking) {
    staticTimeTaskList.push_back(new TaskClass(taskId, calculateExecuteTime(timeToExecute), clss,
                                               skippOtherTasks, isBlocking, true));
    taskId++;
}

void
Scheduler::addNewTaskOnThreadCallingAt(time_t timeToExecute, void (*execFun)(), bool skippOtherTasks, bool isBlocking) {
    addNewTaskOnThreadCallingAt(chrono::system_clock::from_time_t(timeToExecute), execFun, skippOtherTasks, isBlocking);
}

void Scheduler::addNewTaskOnThreadCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool skippOtherTasks,
                                            bool isBlocking) {
    addNewTaskOnThreadCallingAt(chrono::system_clock::from_time_t(timeToExecute), clss, skippOtherTasks, isBlocking);
}

void Scheduler::addNewTaskOnThreadCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                            void (*execFun)(), bool skippOtherTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw Exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskOnThreadCallingAt(nowTimeT, execFun, skippOtherTasks, isBlocking);
}

void Scheduler::addNewTaskOnThreadCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                            TaskClassInterface *clss, bool skippOtherTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw Exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskOnThreadCallingAt(nowTimeT, clss, skippOtherTasks, isBlocking);
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
            throw Exception("Set end time before start time");
    }

    endWorkingTime = date;
    flgEndWorkTimeEnabled = true;
}

void Scheduler::setMaxTimeGap(chrono::minutes minutes) {
    if (minutes > 70min)
        throw Exception("Trying set to big max time gap. Max value is 70minutes");

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
            throw Exception("At setEndWorkingTime passed not all date. Maybe missing day or month and day.");
        }
    } else {
        if (!checkCorrectTime(hour, minute, second))
            throw Exception("Passed time to setEndWorkTime is incorrect");

        if (!checkCorrectDate(year, month, day))
            throw Exception("Passed date to setEndWorkingTime is incorrect");

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
            throw Exception("Passed time to setEndWorkingTime is in the past");

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
        while (true) {
            if (checkEndLoop(&schedulerQueueRepeatable, &schedulerQueueStaticTime)) {
                break;
            } else {
                runLoop();
            }
        }
    } else {
//        cout << chrono::system_clock::to_time_t(now) << " - " << chrono::system_clock::to_time_t(endWorkingTime)
//             << endl;
        while (now < this->endWorkingTime) {
            //this->schedulerQueueRepeatable.printTasksId();
            if (checkEndLoop(&schedulerQueueRepeatable, &schedulerQueueStaticTime)) {
                break;
            } else {
                runLoop();
            }
        }
    }
}

inline bool Scheduler::checkEndLoop(SchedulerQueue *repeatable, SchedulerQueue *staticTime) {
    return (!repeatable->isExisting() && flgEndWhenRepeatableEnd) ||
           (!repeatable->isExisting() && !staticTime->isExisting());
}

void Scheduler::prepareRun() {
    if (flgEndWhenRepeatableEnd) {
        if (repeatableTaskList.empty())
            throw Exception("Repeatable list is empty. Set flg endWhenRepeatableEnd = true");
    } else {
        if (repeatableTaskList.empty() && staticTimeTaskList.empty())
            throw Exception("Task lists is empty. No added tasks to list!");
    }

    prepareRunRepeatable();
    prepareRunStaticTime();

    taskCollector = thread(&Scheduler::taskCollectorFunction, this);
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
    if (schedulerQueueStaticTime.isExisting()) {
        if (now >= schedulerQueueStaticTime.getExecTime()) {
            Task *tsk = schedulerQueueStaticTime.getTask();
            tsk->incrementStaticExecuteTime();
            schedulerQueueStaticTime.addTask(tsk, tsk->getStaticExecuteTime());

            executeTask(tsk);

            if (tsk->getSkipOtherTasks()) {
                schedulerQueueRepeatable.clearQueue();
                prepareRunRepeatable();
            }
            schedulerQueueStaticTime.next();
            return true;
        }
    }
    return false;
}

void Scheduler::runRepeatable() {
    schedulerQueueRepeatable.addActual();

    if (now - schedulerQueueRepeatable.getExecTime() > -1s && schedulerQueueRepeatable.isExisting()) {
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

// Skipping if next is the same (change pointer in queue)
// Skipping if next execute of this task have time lower than now
void Scheduler::skippingTasks() {
    if (schedulerQueueRepeatable.nextIsTheSame()) {
        schedulerQueueRepeatable.next();
    } else {
        if (schedulerQueueRepeatable.nextThisTaskIsExisting()) {
            if (now - schedulerQueueRepeatable.getNextThisTaskExecTime() < 0s) {
                executeRepeatableTask();
            } else {
                //TODO analyze starve process
                schedulerQueueRepeatable.next();
            }
        } else {
            executeRepeatableTask();
        }
    }
}

void Scheduler::executeRepeatableTask() {
    executeTask(this->schedulerQueueRepeatable.getTask());
    this->schedulerQueueRepeatable.next();
}

void Scheduler::executeTask(Task *task) {
    if (task->isBlocking()) {
        while (!runningTaskOnThread.empty()) {
            this_thread::sleep_for(waitForEndTaskOnThreadTime);
        }
        task->execute();
    } else {
        if (task->isRunOnThread()) {
            while (thisTaskIsRunning(task->getId())) {
                this_thread::sleep_for(waitForEndTaskOnThreadTime);
            }
            runningTaskOnThreadLocker.lock();
            runningTaskOnThread.push_back(new TaskOnThread(task));
            runningTaskOnThreadLocker.unlock();
        } else {
            task->execute();
        }
    }
}

chrono::milliseconds Scheduler::calcSleepTime() {
    system_clock_time execTime;
    if (schedulerQueueRepeatable.isExisting()) {
        if (schedulerQueueStaticTime.isExisting()) {
            if (schedulerQueueRepeatable.getExecTime() >= schedulerQueueStaticTime.getExecTime()) {
                execTime = schedulerQueueStaticTime.getExecTime();
            } else {
                execTime = schedulerQueueRepeatable.getExecTime();
            }
        } else {
            execTime = schedulerQueueRepeatable.getExecTime();
        }
    } else {
        if (schedulerQueueRepeatable.getExecTime() >= schedulerQueueStaticTime.getExecTime()) {
            execTime = schedulerQueueStaticTime.getExecTime();
        }
    }
    chrono::milliseconds sleepTime = chrono::duration_cast<chrono::milliseconds>(execTime - now);
    if (sleepTime > maxTimeGap || sleepTime < -6h) {
        throw Exception("Error calculating sleep time! Sleep time: " + to_string(sleepTime.count()) +
                        " ms (max time gap" + to_string(maxTimeGap.count()) + " s, max accepted delay -6h)");
    } else if (sleepTime < 0ms)
        sleepTime = 0ms;

    return sleepTime;
}

bool Scheduler::thisTaskIsRunning(unsigned int id) {
    for (auto &row : runningTaskOnThread) {
        if (row->getTaskId() == id) {
            return true;
        }
    }
    return false;
}

void Scheduler::taskCollectorFunction() {
    while (!flgEndTaskCollector) {
        this_thread::sleep_for(
                waitForEndTaskOnThreadTime + waitForEndTaskOnThreadTime * 100 * runningTaskOnThread.empty());
        for (int i = 0; i < runningTaskOnThread.size(); ++i) {
            if (runningTaskOnThread[i]->taskEnd()) {
                TaskOnThread *temp;
                runningTaskOnThreadLocker.lock();
                temp = runningTaskOnThread[i];
                runningTaskOnThread.erase(runningTaskOnThread.begin() + i);
                runningTaskOnThreadLocker.unlock();
                delete temp;
                --i;
            }
        }
    }
}


Scheduler::TaskOnThread::TaskOnThread(Task *task) {
    this->task = task;
    th = thread(&Scheduler::TaskOnThread::threadTask, this);

}

void Scheduler::TaskOnThread::threadTask() {
    task->execute();
    working = false;
}

bool Scheduler::TaskOnThread::taskEnd() {
    return !working;
}

Scheduler::TaskOnThread::~TaskOnThread() {
    if (th.joinable())
        th.join();
}

unsigned int Scheduler::TaskOnThread::getTaskId() {
    return task->getId();
}
