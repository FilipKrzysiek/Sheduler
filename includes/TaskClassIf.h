//
// Created by krzysiu on 21.09.2019.
//

#ifndef LOGLIB_TASKCLASSIF_H
#define LOGLIB_TASKCLASSIF_H

class TaskClassIf: public Task{
public:
    TaskClassIf(unsigned int id, unsigned int interval, TaskClassInterface *clss, bool canSkipped = true, unsigned int endAfter = 0);
    virtual ~TaskClassIf(){};
    void execute() override ;

private:
    TaskClassInterface *clss;
};

#endif //LOGLIB_TASKCLASSIF_H
