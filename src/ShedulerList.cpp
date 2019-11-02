#include "../includes/ShedulerList.h"

ShedulerList::ShedulerList() {
    this->planedExec = 3;
    this->nextExisting = false;
}

ShedulerList::ShedulerList(unsigned int planedExec) {
    this->planedExec = planedExec;
    this->nextExisting = false;
}

ShedulerList::~ShedulerList() {
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

bool ShedulerList::next() {
    if (this->isExisting()) {
        //This destructing interval, because when we have 3 planned task and one executing more time
        //calculating time uwzględni this in next, next task, but earlier don't know about this.
        //this->addActual();
        waitTask *temp;
        temp = this->wTaskActual->next;
        delete this->wTaskActual;
        this->wTaskActual = temp;
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

Task *ShedulerList::getTask() {
    //if (this->nextExisting)
    return this->wTaskActual->task;
    //else
    //    return nullptr;
}

time_t ShedulerList::getExecTime() {
    if (this->isExisting())
        return this->wTaskActual->timeExec;
    else
        return 0;
}

time_t ShedulerList::getNextExecTime() {
    if (this->nextIsExisting())
        return this->wTaskActual->next->timeExec;
    else
        return 0;
}

bool ShedulerList::nextIsTheSame() {
    if (this->isExisting() && this->nextExisting) {
        waitTask *next = this->wTaskActual->next;
        return this->wTaskActual->task->getId() == next->task->getId();
    } else
        return false;
}

bool ShedulerList::nextIsExisting() {
    if (this->wTaskActual->next == nullptr) {
        this->nextExisting = false;
        return false;
    } else {
        this->nextExisting = true;
        return true;
    }
}

bool ShedulerList::isExisting() {
    if (this->wTaskActual == nullptr) {
        this->nextExisting = false;
        return false;
    } else {
        this->nextExisting = true;
        return true;
    }
}

void ShedulerList::setAmountPlanedExec(unsigned int ne) {
    this->planedExec = ne;
}

void ShedulerList::printTasksId() {
    waitTask *temp;
    temp = this->wTaskActual;
    while (temp != nullptr) {
        std::cout << "Tsk id:" << temp->task->getId() << std::endl;
        temp = temp->next;
    }
}

string ShedulerList::getTaskTimeList() {
    waitTask *temp;
    struct tm *lt;
    string txt = "Task time list: ";
    char buf[15];
    temp = this->wTaskActual;
    while (temp != nullptr) {
        lt = localtime(&temp->timeExec);
        txt += to_string(temp->task->getId());
        txt += " - ";
        strftime(buf, 15, "%T", lt);
        txt += buf;
        txt += "; ";
        temp = temp->next;
    }

    return txt;
}

/*
void ShedulerList::add(Task *tsk, time_t timeExec) {
    if (isExisting()) {
        waitTask *iter = this->wTaskActual, *prev = nullptr;

        //prev = NULL;
        //Inserting
        if (iter->timeExec > timeExec) {
            this->wTaskActual = new waitTask(tsk, timeExec, iter);
            return;
        }

        prev = iter;
        iter = iter->next;

        while (iter != nullptr) {
            if (iter->timeExec > timeExec) {
                prev->next = new waitTask(tsk, timeExec, iter);
                return;
            } else if (iter->timeExec == timeExec) {
                if (iter->next->task->getId() == tsk->getId()) {
                    prev = iter;
                    iter = iter->next;
                    prev->next = new waitTask(tsk, timeExec, iter);
                    return;
                } else {
                    prev->next = new waitTask(tsk, timeExec, iter);
                    return;
                }
            }

            prev = iter;
            iter = iter->next;
        }

        prev->next = new waitTask(tsk, timeExec);
    } else {
        this->wTaskActual = new waitTask(tsk, timeExec);
    }
}*/

void ShedulerList::add(Task *tsk, time_t timeExec) {
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

void ShedulerList::addActual() {
    Task *tsk;
    tsk = this->getTask();
    time_t nextExecTime, now;
    time(&now);
    if (tsk != nullptr) {
        waitTask *wTask = wTaskActual;
        time_t lastTime = wTaskActual->timeExec;
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
        if (tsk->getEndTime() > nextExecTime)
            this->add(tsk, nextExecTime);
    }
}
