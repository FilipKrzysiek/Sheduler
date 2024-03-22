#ifndef TASK_H
#define TASK_H

#include "TaskStaticTime.h"
#include "TaskRepeatable.h"
#include "Exception.h"

class TaskTypeInterface {
public:
				TaskTypeInterface() = default;
    virtual ~TaskTypeInterface() = default;

    /**
     * Execute method, this must be overrided.
     */
    virtual void execute() = 0;
};

#endif // TASK_H
