//
// Created by krzysiu on 21.09.2019.
//

#ifndef LOGLIB_TASKCLASSIF_H
#define LOGLIB_TASKCLASSIF_H
using namespace std::chrono_literals;

class TaskClass : public Task {
public:
    TaskClass(unsigned int id, std::chrono::seconds interval, TaskClassInterface *clss, bool canSkipped = true,
              std::chrono::seconds endAfter = 0s);

    virtual ~TaskClass() {};

    void execute() override;

private:
    TaskClassInterface *clss;
};

#endif //LOGLIB_TASKCLASSIF_H
