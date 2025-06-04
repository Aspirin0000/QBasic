//
// Created by 26403 on 25-1-2.
//

#ifndef CHECKISVALID_H
#define CHECKISVALID_H

#include <string>

class checkIsValid {
public:
    checkIsValid(std::string s) : str(s) {}
protected:
    std::string str;
};

class checkVariantName : public checkIsValid {
public:
    checkVariantName(std::string s) : checkIsValid(s) {}
    bool checkVariantNameValid();
};

#endif //CHECKISVALID_H
