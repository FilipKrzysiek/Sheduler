#ifndef TASKFUNCTION_H
#define TASKFUNCTION_H

using namespace std::chrono_literals;

class TaskFunction : public Task {
public:
    TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped = true,
                 std::chrono::seconds endAfter = 0s);

    virtual ~TaskFunction() {};

    void execute() override;

protected:

private:
    void (*execFun)();
};

#endif // TASKFUNCTION_H
