//
// Created by cysiu on 30.10.2019.
//

#ifndef LIBRARIES1_EXCEPTION_H
#define LIBRARIES1_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;

namespace scheduler {
    class exception : public std::exception {
        string _msg;
    public:
        exception(const string &msg) : _msg(msg) {}

        virtual const char *what() const noexcept override {
            return _msg.c_str();
        }

        virtual string msg() const noexcept {
            return _msg;
        }
    };
}



#endif //LIBRARIES1_EXCEPTION_H
