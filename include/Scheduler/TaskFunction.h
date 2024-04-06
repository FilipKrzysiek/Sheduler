#ifndef TASKFUNCTION_H
#define TASKFUNCTION_H

#include <chrono>
#include "TaskTypeInterface.h"

using namespace std::chrono_literals;


class TaskFunction : public TaskTypeInterface {
public:
    /**
     * Create function task
     * @param execFun function to execute
     */
    TaskFunction(void (*execFun)());

    virtual ~TaskFunction() {};

    void execute() override;

protected:

private:
    void (*execFun)();
};

#endif // TASKFUNCTION_H
