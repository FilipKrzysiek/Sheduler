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
    cout << currentDateTime() << "\t x \t\t\t\t\t My function" << endl;
}

void myFunctionEndAfter() {
    cout << currentDateTime() << "\t\t x\t\t\t \t My function end after" << endl;
}

void myFunctionSkippable() {
    cout << currentDateTime() << "\t\t\t x \t\t\t My skippable function" << endl;
}

void myFunctionCallingAt() {
    cout << currentDateTime() << "\t\t\t\t\t x \t Caling at <----------------------" << endl;
    this_thread::sleep_for(5s);
}

class ExampleClass: public TaskClassInterface {
public:
    void execute() override {
        cout << currentDateTime() << "\t\t\t\t x \t\t class task" << endl;
    }
};

class ExampleClassNew {
public:
    void method() {
        cout << currentDateTime() << "\t\t\t\t _ \t\t bind task: method" << endl;
    }

    void methodParams(int k, double d) {
        cout << currentDateTime() << "\t\t\t\t _ \t\t bind task: method params: " << k << " " << d << endl;
    }
};

void myFunctionNew() {
    cout << currentDateTime() << "\t _ \t\t\t\t\t bind task: My function" << endl;
}

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
    scheduler.addNewTask(2s, &ex, false, false);
    scheduler.addNewTask(2s, myFunctionSkippable, true, true);
    scheduler.addNewTaskCallingAt(chrono::system_clock::now() + 3s, myFunctionCallingAt, false, true);

    ExampleClassNew exN;
    scheduler.addNewTask(10s, TaskBind{myFunctionNew});
    scheduler.addNewTask(10s, TaskBind{&ExampleClassNew::method, exN});
    scheduler.addNewTask(10s, TaskBind{&ExampleClassNew::methodParams, exN, 51, 3.14});

    cout << "Start scheduler" << endl;

    scheduler.run();

    cout << "Closing app" << endl;

    return 0;
}