#ifndef TASK_H
#define TASK_H


class Task
{
    public:
        //Task(unsigned int id, unsigned int interval, void (*execFun)(), bool canSkipped, unsigned int endAfter){};
        //virtual Task(unsigned int interval, string execString, bool canSkipped = true, unsigned int endAfter = 0);
        Task(){};
        virtual ~Task(){};
        virtual void execute() = 0;

        //Getters
        time_t getLastExecute();
        bool getCanSkipped();

        /**
         * @brief Check pass end work time
         * @return
         */
        bool outOfTime();
        time_t getEndTime();
        unsigned int getId();
        unsigned int getInterval();

protected:
    unsigned int id;
    unsigned int interval;
    bool canSkipped;
    time_t now;
    time_t endWorkTime;
    time_t lastExecute;
private:

    void (*taskFun)();

};

#endif // TASK_H
