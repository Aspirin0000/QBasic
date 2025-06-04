//
// Created by 26403 on 25-1-2.
//

#include "checkIsValid.h"
#include <set>

static const std::set<std::string> keywords = {
    "REM", "LET", "PRINT", "INPUT", "GOTO", "IF", "END", "RUN", "THEN", "LOAD", "LIST", "CLEAR", "HELP", "QUIT"
};

bool checkVariantName::checkVariantNameValid() {
    if (keywords.find(str) != keywords.end()) {
        return false;
    }

    if (str.empty()) {
        return false;
    }

    if (!std::isalpha(str[0]) && str[0] != '_') {
        return false;
    }

    for (size_t i = 1; i < str.length(); ++i) {
        if (!std::isalnum(str[i]) && str[i] != '_') {
            return false;
        }
    }

    return true;
}