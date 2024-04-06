//
// Created by Filip on 22.03.2024.
//

#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H
#include <chrono>
#include <memory>
#include "TaskTypeInterface.h"

#include "Exception.h"

using tp_system_clock = std::chrono::time_point<std::chrono::system_clock>;

using namespace std::chrono_literals;

class TaskController {
protected:
    const unsigned int id;
    std::chrono::time_point<std::chrono::system_clock> lastExecute;
    bool blocking;
    const bool runOnThread;

    std::unique_ptr<TaskTypeInterface> task;

public:
    /**
     *
     * @param task task. It will be keept in unique_ptr
     * @param id
     * @param blocking
     * @param run_on_thread
     */
    TaskController(TaskTypeInterface *task, unsigned int id, bool blocking, bool runOnThread);

    /**
     *
     * @param task
     * @param id
     * @param blocking
     * @param run_on_thread
     */
    TaskController(std::unique_ptr<TaskTypeInterface> task, unsigned int id, bool blocking, bool runOnThread);

    virtual ~TaskController() = default;

    /**
     * Get last execute task time.
     * @return Time of last execute
     */
    tp_system_clock getLastExecute() const;

    /**
     * Get task id.
     * @return id task
     */
    unsigned int getId() const;

    /**
     * Check task is blocking, must wait to end other tasks running on other threads.
     * @return true - task must wait to end all other tasks; false - task don't wait for other tasks
     */
    bool isBlocking() const;

    /**
     * Check task is run on master thread or on separate thread.
     * @return true - task run on separate thread; false - task run on main thread
     */
    bool isRunOnThread() const;

    virtual void execute();
};


#endif //TASKCONTROLLER_H
