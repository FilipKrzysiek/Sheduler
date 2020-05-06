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
        std::chrono::time_point<std::chrono::system_clock> getLastExecute();
        bool getCanSkipped();

        /**
         * @brief Check pass end work time
         * @return
         */
        bool outOfTime();
        std::chrono::time_point<std::chrono::system_clock> getEndTime();
        unsigned int getId();
        std::chrono::seconds getInterval();

protected:
    unsigned int id;
    std::chrono::seconds interval;
    bool canSkipped;
    std::chrono::time_point<std::chrono::system_clock> endWorkTime, lastExecute;
private:

    void (*taskFun)();

};

#endif // TASK_H
