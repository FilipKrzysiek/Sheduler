#include "Scheduler.h"


using namespace std;

//TODO delete ended task from list
//TODO task adaptative (calc time from end last task)

Scheduler::Scheduler() {
    this->taskId = 0;
    this->planedExec = 3;
}

template<class Rep, class Period>
Scheduler::Scheduler(chrono::duration<Rep, Period> endAfter) {
    this->taskId = 0;
    this->planedExec = 3;
    setEndWorkTimeAfter(endAfter);
}

template<class Rep, class Period>
void Scheduler::addNewTask(chrono::duration<Rep, Period> interval, void (*execFun)(), bool runOnThread, bool canSkipped,
                           bool isBlocking, chrono::duration<Rep, Period> endAfter) {
    repeatableTaskList.emplace_back(new TaskRepeatable(new TaskFunction(execFun), taskId, interval, isBlocking,
                                                       runOnThread, canSkipped, endAfter));
    taskId++;
}

template<class Rep, class Period>
void Scheduler::addNewTask(chrono::duration<Rep, Period> interval, TaskClassInterface *clss, bool runOnThread,
                           bool canSkipped, bool isBlocking, chrono::duration<Rep, Period> endAfter) {
    repeatableTaskList.emplace_back(new TaskRepeatable(new TaskClass(clss), taskId, interval, isBlocking, runOnThread,
                                                       canSkipped, endAfter));
    taskId++;
}

Scheduler::~Scheduler() {
    flgEndTaskCollector = true;
    if (taskCollector.joinable())
        taskCollector.join();
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(), bool runOnThread,
                                    bool skippOtherTasks, bool isBlocking) {
    staticTimeTaskList.emplace_back(new TaskStaticTime(new TaskFunction(execFun), taskId, timeToExecute, isBlocking,
                                                       runOnThread, skippOtherTasks));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss, bool runOnThread,
                                    bool skippOtherTasks, bool isBlocking) {
    staticTimeTaskList.emplace_back(new TaskStaticTime(new TaskClass(clss), taskId, timeToExecute, isBlocking,
                                                       runOnThread, skippOtherTasks));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool runOnThread, bool skippOtherTasks,
                                    bool isBlocking) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), execFun, runOnThread, skippOtherTasks,
                        isBlocking);
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool runOnThread,
                                    bool skippOtherTasks, bool isBlocking) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), clss, runOnThread, skippOtherTasks,
                        isBlocking);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    void (*execFun)(),
                                    bool runOnThread, bool skippOtherTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw Exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, execFun, runOnThread, skippOtherTasks, isBlocking);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    TaskClassInterface *clss, bool runOnThread, bool skippOtherTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw Exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, clss, runOnThread, skippOtherTasks, isBlocking);
}

template<class Rep, class Period>
void Scheduler::setEndWorkTimeAfter(chrono::duration<Rep, Period> time) {
    this->endWorkingTime = chrono::system_clock::now() + time;
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

template<class Rep, class Period>
void Scheduler::setMaxTimeGap(chrono::duration<Rep, Period> time) {
    maxTimeGap = time;
}

string Scheduler::getTaskTimeList() {
    return schedulerQueueRepeatable.getTaskTimeList();
}

chrono::microseconds Scheduler::getSleept() {
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

template<class Rep, class Period>
void Scheduler::setDelayBetweenTasks(const chrono::duration<Rep, Period> &delayBetweenTasks) {
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
            throw Exception("Repeatable list is empty. When set flg endWhenRepeatableEnd = true");
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

    TaskRepeatable *tsk;
    for (int i = 0; i < repeatableTaskList.size(); i++) {
        tsk = repeatableTaskList.at(i).get();
        for (int j = 0; j < planedExec; j++) {
            system_clock_time timeTo = j * tsk->getInterval() + now + delayBetweenTasks * i;
            if (tsk->getIsNeverEnding() || tsk->getEndTime() > timeTo)
                schedulerQueueRepeatable.addTask(tsk, timeTo);
        }
    }
}

void Scheduler::prepareRunStaticTime() {
    schedulerQueueStaticTime.setAmountPlanedExec(1);

    for (auto &tsk: staticTimeTaskList) {
        if (now > tsk->getStaticExecuteTime())
            tsk->incrementStaticExecuteTime();

        schedulerQueueStaticTime.addTask(tsk.get(), tsk->getStaticExecuteTime());
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
            TaskStaticTime *tsk = dynamic_cast<TaskStaticTime*>(schedulerQueueStaticTime.getTask());
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
                if (dynamic_cast<TaskRepeatable*>(schedulerQueueRepeatable.getTask())->getCanSkipped()) {
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

void Scheduler::executeTask(TaskController *task) {
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
    for (auto &row: runningTaskOnThread) {
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
                runningTaskOnThreadLocker.lock();
                TaskOnThread *temp = runningTaskOnThread[i];
                runningTaskOnThread.erase(runningTaskOnThread.begin() + i);
                runningTaskOnThreadLocker.unlock();
                delete temp;
                --i;
            }
        }
    }
}


Scheduler::TaskOnThread::TaskOnThread(TaskController *task) {
    this->task = task;
    th = thread(&Scheduler::TaskOnThread::threadTask, this);
}

void Scheduler::TaskOnThread::threadTask() {
    task->execute();
    working = false;
}

bool Scheduler::TaskOnThread::taskEnd() const {
    return !working;
}

Scheduler::TaskOnThread::~TaskOnThread() {
    if (th.joinable())
        th.join();
}

unsigned int Scheduler::TaskOnThread::getTaskId() const {
    return task->getId();
}
