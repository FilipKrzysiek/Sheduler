//
// Created by Filip on 22.03.2024.
//

#include "../include/Scheduler/TaskController.h"

/**
 *
 * @param task
 * @param id task id
 * @param blocking if true all tasks run on thread must end before start this task.
 * @param runOnThread run this task on separate thread
 */
TaskController::TaskController(TaskTypeInterface *task, unsigned int id, bool blocking, bool runOnThread)
    : id(id), blocking(blocking), runOnThread(runOnThread), task(task) {
}

/**
 *
 * @param task
 * @param id task id
 * @param blocking if true all tasks run on thread must end before start this task.
 * @param runOnThread run this task on separate thread
 */
TaskController::TaskController(std::unique_ptr<TaskTypeInterface> task, unsigned int id, bool blocking, bool runOnThread)
    : id(id), blocking(blocking), runOnThread(runOnThread), task(std::move(task)) {
}

std::chrono::time_point<std::chrono::system_clock> TaskController::getLastExecute() const {
    return lastExecute;
}

unsigned int TaskController::getId() const {
    return id;
}

bool TaskController::isBlocking() const {
    return blocking;
}

bool TaskController::isRunOnThread() const {
    return runOnThread;
}

void TaskController::execute() {
    lastExecute = std::chrono::system_clock::now();
    task->execute();
}
