#ifndef TASKFUNCTION_H
#define TASKFUNCTION_H


class TaskFunction : public Task
{
    public:
        TaskFunction(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped = true, unsigned int endAfter = 0);
        virtual ~TaskFunction(){};
        void execute() override ;

    protected:

    private:
    void (*execFun)();
};

#endif // TASKFUNCTION_H
