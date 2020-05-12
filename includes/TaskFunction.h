#ifndef TASKFUNCTION_H
#define TASKFUNCTION_H

using namespace std::chrono_literals;


class TaskFunction : public Task {
public:
    /**
     *
     * @param id
     * @param interval
     * @param execFun
     * @param canSkipped
     * @param endAfter
     */
    TaskFunction(unsigned int id, std::chrono::seconds interval, void (*execFun)(), bool canSkipped = true,
                 std::chrono::seconds endAfter = 0s);

    TaskFunction(unsigned int id, std::chrono::time_point<std::chrono::system_clock> executeTime, void (*execFun)());

    virtual ~TaskFunction() {};

    void execute() override;

protected:

private:
    void (*execFun)();
};

#endif // TASKFUNCTION_H
