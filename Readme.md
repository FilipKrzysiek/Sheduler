# Task scheduler library

Simple library for scheduling tasks in c++, created in c++17 and basing on stl library.

## Development environment

Library was created under ubuntu on WSL. Should work on all systems because not used system libraries.

Public functions has created doxygen and in doc directory is doxygen config file to generate documentation (I still learn it).

## Features

- Two types of tasks:
  - Repeatable task
  - Called at specified time task
- Task can be:
  - Blocking (other working tasks must end before start this task)
  - Working on thread
  - Skipped if delayed
  - End working after specified time
- Task types:
  - Function
  - Class inherit from template class
  - Element passed like to `std::thread`
- Scheduler can end work at specified time or after a certain period
- Can be specified max time gap between tasks, if is grater than it scheduler throw error
- Delay between starting tasks (first start, and second start 100ms later)
- Run blocking or not blocking

## How to use

1. Create scheduler object.
2. Set scheduler options.
3. Add tasks.
4. Run scheduler.

### Create scheduler object

We can create scheduler using one of two constructors:

- Non-parametric constructor - default never ending
- With `endAfter` parameter 

### Set scheduler options

- ```void setEndWorkTimeAfter (chrono::microseconds time)``` - end after specified time
- ```void setEndWorkTime (system_clock_time date, bool getOnlyTime=true)``` - set date and time when scheduler should stop working (or only time if flag `getOnlyTime` is `true`)
- ```void setEndWorkTime (time_t date, bool getOnlyTime=true)```- the same but with `time_t`
- ```void setEndWorkTime (unsigned short hour, unsigned short minute, unsigned short second, unsigned short year=65535, unsigned short month=65535, unsigned short day=65535)``` - the same, but when not passed year/month/day, get actual
- ```void setMaxTimeGap (chrono::microseconds time)``` - set max time gap between tasks. If time gap is grater than set, throw exception
- ```void setEndWhenRepeatableEnd (bool flg)``` - when all repeatable end, scheduler stop working, if false and repeatable queue is empty and static time task still exists it's still work 
- ```void setDelayBetweenTasks (const chrono::microseconds &delayBetweenTasks)``` - delay between tasks (that all passed tasks not start on the same time)
- ```void setAcceptedDelay (const chrono::microseconds &acceptedDelay)``` - accepted delay is delay that is not delay 😉

Default values:

| Option                  | Value                              |
|-------------------------|------------------------------------|
| End work time           | Never ending                       |
| Max time gap            | 2 hours                            |
| End when repeatable end | End when repeatable queue is empty |
| Delay Between tasks     | 1s                                 |
| Accepted delay          | 1s                                 |




### Add tasks

**Repeatable task**

```c++
void addNewTask(chrono::microseconds interval, void(*execFun)(), bool runOnThread=true, bool canSkipped=true, bool isBlocking=false, chrono::microseconds endAfter=0s);
void addNewTask(chrono::microseconds interval, TaskClassInterface *clss, bool runOnThread = true, bool canSkipped = true, bool isBlocking = false, chrono::microseconds endAfter = 0s);
void addNewTask(chrono::microseconds interval, TaskBind taskBind, bool runOnThread = true, bool canSkipped = true, bool isBlocking = false, chrono::microseconds endAfter = 0s);
```

Parameters:

- `interval` - interval with task will be called in chrono time 
- task:
  - pointer to function
  - pointer to class which inherits `TaskClassInterface` (it will be described later)
  - `TaskBind` object (it will be described later)
- `runOnThread` - run this task on thread
- `canSkipped` - Call can be skipped when is delayed or when the same task still work
- `isBlocking` - if true all tasks run on thread must end before start this task
- `endAfter` - Ending after seconds. If 0 never ending

**Static time task**

```c++
void addNewTaskCallingAt(system_clock_time timeToExecute, void (*execFun)(), bool recalcRepTasks = true, bool isBlocking = false);
void addNewTaskCallingAt(system_clock_time timeToExecute, TaskClassInterface *clss, bool recalcRepTasks = true, bool isBlocking = false);
void addNewTaskCallingAt(time_t timeToExecute, void (*execFun)(), bool recalcRepTasks = true, bool isBlocking = false);
void addNewTaskCallingAt(time_t timeToExecute, TaskClassInterface *clss, bool recalcRepTasks = true, bool isBlocking = false);
void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second, void (*execFun)(), bool recalcRepTasks = true, bool isBlocking = false);
void addNewTaskCallingAt(unsigned short hour, unsigned short minute, unsigned short second, askClassInterface *clss, bool recalcRepTasks = true, bool isBlocking = false);
```

Parameters:

- time when to call:
  - in `chrono::time_point<chrono::system_clock>` 
  - in `time_t`
  - in separated values
- task:
  - pointer to function
  - pointer to class which inherits `TaskClassInterface` (it will be described later)
  - `TaskBind` object (it will be described later) (it will be added later)
- `recalcRepTasks` - (recalculateRepeatableTasksTime) this option works only when @blocking = true. This change behaviour of recalculating repeatable task execution time. When true - all outdated tasks will be skipped, if false - execution time of all task will be recalculated, after end this task
- `isBlocking` - if true all tasks run on thread must end before start this task and all another tasks will wait until this task end

### Task types

**Pointer to functions**

Simple pointer to functions.

**Inheritance of template class**

Your class should inherit `TaskClassInterface` and override `execute()` method.

**TaskBind**

This is task wrapper to call function, class, lambda like `std::thread`. Example:

```c++
void myFunctionNew();
void myFunctionWithParams(int a, int b);

class MyClass {
public:
    void myMethod();
};

//Function call
TaskBind tb1{myFunctionNew};

//Function call with function params
TaskBind tb2{myFunctionWithParams, 5, 8};

//Call method of class
MyClass myClass;
TaskBind tb3{&myClass, &MyClass::myMethod};
```

### Run scheduler

Let's define scheduler:

```c++
Scheduler scheduler;
//add tasks
```

&nbsp;

Run scheduler blocking:

```c++
scheduler.run();
```

Run scheduler non-blocking:

```c++
scheduler.start();

//Wait to end
scheduler.wait();
```

Force stop working:

```c++
scheduler.stop();
```

Checking that scheduler is working:

```c++
scheduler.isFlgWorking();
```

## Examples

Search examples [here](./example)

## License

Free to use and modify, only link to this repository.

