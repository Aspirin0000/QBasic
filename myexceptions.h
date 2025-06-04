//
// Created by 26403 on 25-1-2.
//

#ifndef MYEXCEPTIONS_H
#define MYEXCEPTIONS_H
#include <exception>
#include <string>


enum ErrorCode {
    INVALIDSTATEMENT = 114514,
    INVALIDEXPRESSION,
    INVALIDVARIANT,
    UNDEFINEDVARIANT,
    INVALIDLINENUMBER,
    NONEXISTENTLINENUMBER
};



class MyException : public std::exception {
private:
    std::string message{};
    int errorCode;
public:
    MyException(const std::string& msg, int code) : message(msg), errorCode(code) {}
    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
    [[nodiscard]] int code() const noexcept {
        return errorCode;
    }
};

#endif //MYEXCEPTIONS_H
