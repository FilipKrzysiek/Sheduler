//
// Created by krzysiu on 21.09.2019.
//

#ifndef TASKCLASSIF_H
#define TASKCLASSIF_H

#include <chrono>
#include "TaskClassInterface.h"
#include "TaskTypeInterface.h"


using namespace std::chrono_literals;


class TaskClass : public TaskTypeInterface {
public:
    /**
     * Create class based task
     * @param clss class inherits TaskClassInterface
     */
    explicit TaskClass(TaskClassInterface *clss);

    virtual ~TaskClass() {
    };

    void execute() override;

private:
    TaskClassInterface *clss;
};

#endif //TASKCLASSIF_H
