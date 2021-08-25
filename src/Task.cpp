#include "Task.h"

std::chrono::time_point<std::chrono::system_clock> Task::getLastExecute() {
    return this->lastExecute;
}

unsigned int Task::getId() {
    return this->id;
}

bool Task::isBlocking() const {
    return blocking;
}

bool Task::isRunOnThread() {
    return runOnThread;
}


