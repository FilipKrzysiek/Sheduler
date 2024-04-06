#ifndef TASK_H
#define TASK_H

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
