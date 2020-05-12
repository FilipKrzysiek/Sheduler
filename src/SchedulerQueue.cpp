#include "../includes/SchedulerQueue.h"

SchedulerQueue::SchedulerQueue() {
    this->planedExec = 3;
    this->nextExisting = false;
}

SchedulerQueue::SchedulerQueue(unsigned int planedExec) {
    this->planedExec = planedExec;
    this->nextExisting = false;
}

SchedulerQueue::~SchedulerQueue() {
    if (this->isExisting()) {
        waitTask *next;
        while (this->wTaskActual->next != nullptr) {
            next = this->wTaskActual->next;
            delete (this->wTaskActual);
            this->wTaskActual = next;
        }
        delete (this->wTaskActual);
    }
}

bool SchedulerQueue::next() {
    if (this->isExisting()) {
        waitTask *temp;
        temp = this->wTaskActual;
        this->wTaskActual = this->wTaskActual->next;
        delete temp;
        if (temp == nullptr) {
            this->nextExisting = false;
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

Task *SchedulerQueue::getTask() {
    //if (this->nextExisting)
    return this->wTaskActual->task;
    //else
    //    return nullptr;
}

chrono::time_point<chrono::system_clock> SchedulerQueue::getExecTime() {
    if (this->isExisting())
        return this->wTaskActual->timeExec;
    else
        return chrono::system_clock::from_time_t(0);
}

chrono::time_point<chrono::system_clock> SchedulerQueue::getNextExecTime() {
    if (this->nextIsExisting())
        return this->wTaskActual->next->timeExec;
    else
        return chrono::system_clock::from_time_t(0);
}

bool SchedulerQueue::nextIsTheSame() {
    if (this->isExisting() && this->nextExisting) {
        waitTask *next = this->wTaskActual->next;
        return this->wTaskActual->task->getId() == next->task->getId();
    } else
        return false;
}

bool SchedulerQueue::nextIsExisting() {
    if (this->wTaskActual->next == nullptr) {
        this->nextExisting = false;
        return false;
    } else {
        this->nextExisting = true;
        return true;
    }
}

bool SchedulerQueue::isExisting() {
    if (this->wTaskActual == nullptr) {
        this->nextExisting = false;
        return false;
    } else {
        this->nextExisting = true;
        return true;
    }
}

void SchedulerQueue::setAmountPlanedExec(unsigned int ne) {
    this->planedExec = ne;
}

void SchedulerQueue::printTasksId() {
    waitTask *temp;
    temp = this->wTaskActual;
    while (temp != nullptr) {
        std::cout << "Tsk id:" << temp->task->getId() << std::endl;
        temp = temp->next;
    }
}

string SchedulerQueue::getTaskTimeList() {
    waitTask *temp;
    struct tm *lt;
    string txt = "Task time list: ";
    char buf[15];
    temp = this->wTaskActual;
    while (temp != nullptr) {
        time_t timeTemp = chrono::system_clock::to_time_t(temp->timeExec);
        lt = localtime(&timeTemp);
        txt += to_string(temp->task->getId());
        txt += " - ";
        strftime(buf, 15, "%T", lt);
        txt += buf;
        txt += "; ";
        temp = temp->next;
    }

    return txt;
}

void SchedulerQueue::addTask(Task *tsk, chrono::time_point<chrono::system_clock> timeExec) {
    if (isExisting()) {
        waitTask *iter = this->wTaskActual, *prev = nullptr;

        //Check existing next
        if (iter->next == nullptr) {
            iter->next = new waitTask(tsk, timeExec);
            return;
        } else {
            //Check time for first task
            if (iter->timeExec > timeExec) {
                this->wTaskActual = new waitTask(tsk, timeExec, iter);
                return;
            } else {
                while (iter->next != nullptr) {
                    prev = iter;
                    iter = iter->next;
                    //Check time
                    if (iter->timeExec > timeExec) {
                        prev->next = new waitTask(tsk, timeExec, iter);
                        return;
                    }
                }
                //If not existing next
                iter->next = new waitTask(tsk, timeExec);
            }
        }
    } else {
        this->wTaskActual = new waitTask(tsk, timeExec);
    }
}

void SchedulerQueue::addActual() {
    Task *tsk;
    tsk = this->getTask();
    chrono::time_point<chrono::system_clock> nextExecTime, now;
    now = chrono::system_clock::now();
    if (tsk != nullptr) {
        waitTask *wTask = wTaskActual;
        chrono::time_point<chrono::system_clock> lastTime = wTaskActual->timeExec;
        wTask = wTask->next;
        //Find last time this task exec on list
        while (wTask != nullptr) {
            if (wTask->timeExec > lastTime && wTask->task->getId() == tsk->getId()) {
                lastTime = wTask->timeExec;
            }
            wTask = wTask->next;
        }

        nextExecTime = lastTime + tsk->getInterval();
        while (nextExecTime < now) {
            nextExecTime += tsk->getInterval();
        }
        if (tsk->getIsNeverEnding() || tsk->getEndTime() > nextExecTime)
            this->addTask(tsk, nextExecTime);
    }
}

void SchedulerQueue::clearQueue() {
    waitTask *wTaskTemp;
    while(wTaskActual != nullptr){
        wTaskTemp = wTaskActual;
        wTaskActual = wTaskActual->next;
        delete wTaskTemp;
    }
    nextExisting = false;
}

bool SchedulerQueue::nextThisTaskIsExisting() {
    waitTask *temp;
    temp = wTaskActual;
    while(temp != nullptr){
        if(temp->task->getId() == wTaskActual->task->getId()){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

chrono::time_point<chrono::system_clock> SchedulerQueue::getNextThisTaskExecTime() {
    waitTask *temp;
    temp = wTaskActual;
    while(temp != nullptr){
        if(temp->task->getId() == wTaskActual->task->getId()){
            return temp->timeExec;
        }
        temp = temp->next;
    }
    return chrono::system_clock::from_time_t(0);
}
