#ifndef TASKFUNCTION_H
#define TASKFUNCTION_H

#include <chrono>
#include "Task.h"

using namespace std::chrono_literals;


class TaskFunction : public Task {
public:
    /**
     * Create repeatable task.
     * @param id task id
     * @param interval time behind this task execution (in seconds)
     * @param execFun address to function which will be executed
     * @param canSkipped Can skipp this task if is delayed
     * @param isBlocking if true all tasks run on thread must end before start this task.
     * @param runOnThread run this task on separate thread.
     * @param endAfter if 0s never ending task, else time after which task will end work (not run again).
     */
    TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped = true,
                 bool isBlocking = false, bool runOnThread = false, std::chrono::seconds endAfter = 0s);

    /**
     * Create static time task
     * @param id task id
     * @param executeTime time when task should be executed
     * @param execFun address to function which will be executed
     * @param skipOtherTasks true - other repeatable task will be skipped, when this task work, false - other task wait to end work this task
     * @param isBlocking if true all tasks run on thread must end before start this task.
     * @param runOnThread run this task on separate thread
     */
    TaskFunction(unsigned int id, std::chrono::time_point<std::chrono::system_clock> executeTime, void (*execFun)(),
                 bool skipOtherTasks = true, bool isBlocking = false, bool runOnThread = false);

    virtual ~TaskFunction() {};

    void execute() override;

protected:

private:
    void (*execFun)();
};

#endif // TASKFUNCTION_H
