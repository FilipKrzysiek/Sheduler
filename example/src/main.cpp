//
// Created by Filip on 24.03.2024.
//

#include <iostream>

#include <Scheduler/Scheduler.h>

using namespace std;

//from https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "[%X] ", &tstruct);

    return buf;
}

void myFunction() {
    cout << currentDateTime() << "\t x \t\t\t\tMy function" << endl;
}

void myFunctionEndAfter() {
    cout << currentDateTime() << "\t\t x\t\t\tMy function end after" << endl;
}

void myFunctionSkippable() {
    cout << currentDateTime() << "\t\t\t x \t\tMy skippable function" << endl;
}

class ExampleClass: public TaskClassInterface {
public:
    void execute() override {
        cout << currentDateTime() << "\t\t\t\t x \tclass task" << endl;
    }
};

int main(int argc, char *argv[])
{
    cout << "Start app" << endl;
    Scheduler scheduler;
    ExampleClass ex;

    cout << "Prepare scheduler" << endl;

    scheduler.setEndWorkTimeAfter(2min);
    scheduler.setDelayBetweenTasks(100ms);
    scheduler.addNewTask(5s, myFunction);
    scheduler.addNewTask(5s, myFunctionEndAfter, false, false, true, 20s);
    scheduler.addNewTask(1s, &ex, false, false);
    scheduler.addNewTask(1s, myFunctionSkippable, true, true);

    cout << "Start scheduler" << endl;

    scheduler.run();

    cout << "Closing app" << endl;

    return 0;
}