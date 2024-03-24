#include "Scheduler.h"


using namespace std;

Scheduler::Scheduler() {
}

Scheduler::Scheduler(chrono::microseconds endAfter) {
    setEndWorkTimeAfter(endAfter);
}

void Scheduler::addNewTask(chrono::microseconds interval, void (*execFun)(), bool runOnThread, bool canSkipped,
                           bool isBlocking, chrono::microseconds endAfter) {
    repeatableTaskList.emplace_back(new TaskRepeatable(new TaskFunction(execFun), taskId, interval, isBlocking,
                                                       runOnThread, canSkipped, endAfter));
    taskId++;
}

void Scheduler::addNewTask(chrono::microseconds interval, TaskClassInterface *clss, bool runOnThread,
                           bool canSkipped, bool isBlocking, chrono::microseconds endAfter) {
    repeatableTaskList.emplace_back(new TaskRepeatable(new TaskClass(clss), taskId, interval, isBlocking, runOnThread,
                                                       canSkipped, endAfter));
    taskId++;
}

Scheduler::~Scheduler() {
    waitToEndAllTasksOnThread();
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(),
                                    bool recalcRepTasks, bool isBlocking) {
    staticTimeTaskList.emplace_back(new TaskStaticTime(new TaskFunction(execFun), taskId, timeToExecute, isBlocking,
                                                       recalcRepTasks));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss,
                                    bool recalcRepTasks, bool isBlocking) {
    staticTimeTaskList.emplace_back(new TaskStaticTime(new TaskClass(clss), taskId, timeToExecute, isBlocking,
                                                       recalcRepTasks));
    taskId++;
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool recalcRepTasks,
                                    bool isBlocking) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), execFun, recalcRepTasks, isBlocking);
}

void Scheduler::addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss,
                                    bool recalcRepTasks, bool isBlocking) {
    addNewTaskCallingAt(chrono::system_clock::from_time_t(timeToExecute), clss, recalcRepTasks, isBlocking);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    void (*execFun)(),
                                    bool recalcRepTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw scheduler::exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, execFun, recalcRepTasks, isBlocking);
}

void Scheduler::addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second,
                                    TaskClassInterface *clss, bool recalcRepTasks, bool isBlocking) {
    if (!checkCorrectTime(hour, minute, second))
        throw scheduler::exception("Passed time to create task AT is incorrect");

    time_t nowTimeT;
    time(&nowTimeT);

    nowTimeT = calculateEndTime(nowTimeT, hour, minute, second);
    addNewTaskCallingAt(nowTimeT, clss, recalcRepTasks, isBlocking);
}

void Scheduler::setEndWorkTimeAfter(chrono::microseconds time) {
    this->endWorkingTime = chrono::system_clock::now() + time;
    flgEndWorkTimeEnabled = true;
}

void Scheduler::setEndWorkTime(system_clock_time date, bool getOnlyTime) {
    if (date < chrono::system_clock::now()) {
        if (getOnlyTime) {
            date = date + 24h;
        } else
            throw scheduler::exception("Set end time before start time");
    }

    endWorkingTime = date;
    flgEndWorkTimeEnabled = true;
}

void Scheduler::setMaxTimeGap(chrono::microseconds time) {
    maxTimeGap = time;
}

// string Scheduler::getTaskTimeList() {
//     return schedulerQueueRepeatable.getTaskTimeList();
// }

// chrono::microseconds Scheduler::getSleept() {
//     return slept;
// }

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

// system_clock_time Scheduler::calculateExecuteTime(system_clock_time executeTime) {
//     time_t timeTNow, timeTExecuteTime;
//     time(&timeTNow);
//     timeTExecuteTime = chrono::system_clock::to_time_t(executeTime);
//     tm *localNow, *localExec;
//     localNow = localtime(&timeTNow);
//     localExec = localtime(&timeTExecuteTime);
//
//     localNow->tm_hour = localExec->tm_hour;
//     localNow->tm_min = localExec->tm_min;
//     localNow->tm_sec = localExec->tm_sec;
//
//     timeTExecuteTime = mktime(localNow);
//
//     return chrono::system_clock::from_time_t(timeTExecuteTime);
// }

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
            throw scheduler::exception("At setEndWorkingTime passed not all date. Maybe missing day or month and day.");
        }
    } else {
        if (!checkCorrectTime(hour, minute, second))
            throw scheduler::exception("Passed time to setEndWorkTime is incorrect");

        if (!checkCorrectDate(year, month, day))
            throw scheduler::exception("Passed date to setEndWorkingTime is incorrect");

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
            throw scheduler::exception("Passed time to setEndWorkingTime is in the past");

        setEndWorkTime(newTimeT, false);
    }
}

void Scheduler::setEndWhenRepeatableEnd(bool flg) {
    flgEndWhenRepeatableEnd = flg;
}

void Scheduler::setDelayBetweenTasks(const chrono::microseconds &delayBetweenTasks) {
    Scheduler::delayBetweenTasks = delayBetweenTasks;
}


//-------------------------------------

void Scheduler::run() {
    updateNow();
    prepareRun();
    if (flgEndWorkTimeEnabled) {
        while (now < endWorkingTime && mainLoopCondition()) {
            runLoop();
        }
    } else {
        while (mainLoopCondition()) {
            runLoop();
        }
    }

    waitToEndAllTasksOnThread();
}

bool Scheduler::mainLoopCondition() {
    if (taskQueue.empty()) {
        return false;
    }

    long amountRepetable = std::count_if(taskQueue.begin(), taskQueue.end(), [](const TaskListItem &elem) {
        return elem.repetableTask == nullptr;
    });
    long amountStatic = std::count_if(taskQueue.begin(), taskQueue.end(),
                                      [](const TaskListItem &elem) { return elem.staticTask == nullptr; });
    return (amountRepetable == 0 && flgEndWhenRepeatableEnd) || (amountRepetable == 0 && amountStatic == 0);
}

void Scheduler::waitToEndAllTasksOnThread() {
    for (auto it = mapTasksOnThread.cbegin(), next_it = it; it != mapTasksOnThread.cend(); it = next_it) {
        ++next_it;
        it->second.wait();
        mapTasksOnThread.erase(it);
    }
}

void Scheduler::updateTasksOnThread() {
    for (auto it = mapTasksOnThread.cbegin(), next_it = it; it != mapTasksOnThread.cend(); it = next_it) {
        ++next_it;
        auto status = it->second.wait_for(0s);

        if (status == future_status::ready) {
            mapTasksOnThread.erase(it);
        }
    }
}

void Scheduler::prepareRun() {
    if (flgEndWhenRepeatableEnd) {
        if (repeatableTaskList.empty())
            throw scheduler::exception("Repeatable list is empty. When set flg endWhenRepeatableEnd = true");
    } else {
        if (repeatableTaskList.empty() && staticTimeTaskList.empty())
            throw scheduler::exception("Task lists is empty. No added tasks to list!");
    }

    preapreRunRepeatableTasks();

    for (auto &task: staticTimeTaskList) {
        while (task->getStaticExecuteTime() < now) {
            task->incrementStaticExecuteTime();
        }

        addTaskToQueue({task, task->getStaticExecuteTime()});
    }
}

void Scheduler::preapreRunRepeatableTasks() {
    for (size_t i = 0; i < repeatableTaskList.size(); ++i) {
        for (unsigned short j = 0; j < planedTasks; ++j) {
            system_clock_time timeTo = now + j * repeatableTaskList[i]->getInterval() + delayBetweenTasks * i;
            addTaskToQueue({repeatableTaskList[i], timeTo});
        }
    }
}

void Scheduler::addTaskToQueue(const TaskListItem &task) {
    if (task.repetableTask != nullptr && task.repetableTask->outOfTime()) {
        return;
    }

    if (taskQueue.empty()) {
        taskQueue.push_back(task);
    } else {
        for (size_t i = 0; i < taskQueue.size(); ++i) {
            if (taskQueue[i].execTime > task.execTime) {
                taskQueue.insert(taskQueue.begin() + i, task);
                return;
            }
        }
        taskQueue.push_back(task);
    }
}

void Scheduler::updateNow() {
    now = chrono::system_clock::now();
}

void Scheduler::runLoop() {
    updateTasksOnThread();
    updateNow();

    auto actualtaskContr = taskQueue.begin();
    auto sleepTime = calcSleepTime();
    this_thread::sleep_for(sleepTime);

    if (actualtaskContr->staticTask != nullptr) {
        executeStaticTimeTask(actualtaskContr);
    } else {
        executeRepeatabletask(actualtaskContr);
    }
}

void Scheduler::executeStaticTimeTask(deque<TaskListItem>::iterator &actualtaskContr) {
    auto task = actualtaskContr->staticTask;

    if (task->isBlocking()) {
        waitToEndAllTasksOnThread();
        task->execute();

        if (!task->getRecalcRepTasks()) {
            for (size_t i = 0; i < taskQueue.size(); ++i) {
                if (taskQueue[i].repetableTask != nullptr) {
                    taskQueue.erase(taskQueue.begin() + i);
                    --i;
                }
            }

            preapreRunRepeatableTasks();
        }
    } else {
        mapTasksOnThread.emplace(task->getId(), std::async(std::launch::async, &TaskStaticTime::execute, task));
    }

    task->incrementStaticExecuteTime();
    addTaskToQueue(*actualtaskContr);
    taskQueue.pop_front();
}

void Scheduler::executeRepeatabletask(deque<TaskListItem>::iterator &actualtaskContr) {
    auto task = actualtaskContr->repetableTask;

    if (task->isBlocking()) {
        waitToEndAllTasksOnThread();
    }

    if (task->isRunOnThread()) {
        auto taskOnThread = mapTasksOnThread.find(task->getId());
        if (taskOnThread != mapTasksOnThread.end()) {
            if (task->getCanSkipped()) {
                repeatRepatableTask(actualtaskContr);
                taskQueue.pop_front();
                return;
            } else {
                taskOnThread->second.wait();
            }
        }

        mapTasksOnThread.emplace(task->getId(), std::async(std::launch::async, &TaskRepeatable::execute, task));
    } else {
        task->execute();
    }

    repeatRepatableTask(actualtaskContr);
    taskQueue.pop_front();
}

void Scheduler::repeatRepatableTask(deque<TaskListItem>::iterator &actualtaskContr) {
    auto task = actualtaskContr->repetableTask;
    if (!task->getIsNeverEnding() && now + planedTasks * task->getInterval() > task->getEndTime()) {
        return;
    }

    actualtaskContr->execTime = actualtaskContr->execTime + planedTasks * task->getInterval();
    addTaskToQueue(*actualtaskContr);
}

chrono::microseconds Scheduler::calcSleepTime() {
    auto sleepTime = taskQueue[0].execTime - now;
    if (sleepTime < 0s) {
        processDelays();

        sleepTime = taskQueue[0].execTime - now;
        if (sleepTime <= 0s) {
            return 0us;
        }
    }

    if (sleepTime > maxTimeGap) {
        throw scheduler::exception("Calculated sleep time is higher than"s + to_string(sleepTime.count()) + "s"s);
    }

    return chrono::duration_cast<chrono::microseconds>(sleepTime);
}

void Scheduler::processDelays() {
    if (taskQueue[0].staticTask != nullptr && now - taskQueue[0].execTime < acceptedDelay && taskQueue.size() > 1) {
        for (size_t i = 1; i < taskQueue.size(); ++i) {
            if (taskQueue[i].repetableTask == nullptr) {
                continue;
            }

            auto thisIterTask = taskQueue[i].repetableTask;
            auto nextTheSameTask = std::find_if(taskQueue.begin() + i, taskQueue.end(),
                                                [&thisIterTask](const TaskListItem &task) {
                                                    return task.repetableTask != nullptr && task.repetableTask->getId()
                                                           == thisIterTask->getId();
                                                });

            if (nextTheSameTask->execTime < now) {
                auto iter = taskQueue.begin() + i;
                repeatRepatableTask(iter);
                taskQueue.erase(taskQueue.begin() + i);
                --i;
            } else {
                break;
            }
        }

        auto actualTask = taskQueue[0].repetableTask;
        if (actualTask->getCanSkipped()) {
            if (taskQueue[1].repetableTask != nullptr && taskQueue[1].repetableTask->getId() == actualTask->getId()) {
                auto iter = taskQueue.begin();
                repeatRepatableTask(iter);
                taskQueue.pop_front();
            }
            //TODO think about skipping if last time wasn't skipped
        }
    }
}
