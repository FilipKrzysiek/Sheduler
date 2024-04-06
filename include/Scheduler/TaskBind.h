//
// Created by Filip on 29.03.2024.
//

#ifndef TASKBIND_H
#define TASKBIND_H
#include <functional>
#include "TaskTypeInterface.h"

class TaskBind : public TaskTypeInterface {
    std::function<void(void)> callbackElement;

public:
    template<typename Callable, typename... Args>
    explicit TaskBind(Callable&& f, Args&&... args): callbackElement(std::bind(f, args...)) {
    }

    TaskBind(const TaskBind &other);

    TaskBind(TaskBind &&other) noexcept;

    TaskBind & operator=(const TaskBind &other) = delete;

    TaskBind & operator=(TaskBind &&other) noexcept = delete;

    ~TaskBind() override = default;

    void execute() override;
};



#endif //TASKBIND_H
