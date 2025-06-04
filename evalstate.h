//
// Created by 26403 on 25-1-2.
//

#ifndef EVALSTATE_H
#define EVALSTATE_H
#include "expression.h"



class evalState {
public:
    evalState() {
        evaluationContext = new EvaluationContext();
    }
    ~evalState() {
        delete evaluationContext;
    }
    void clear() {
        delete evaluationContext;
        evaluationContext = new EvaluationContext();
    }
    void setValue(const string& variant, int value) {
        evaluationContext -> setValue(variant, value);
    }
    int getValue(const string& variant);
    EvaluationContext* getEvaluationContext() {return evaluationContext;}
private:
    EvaluationContext*  evaluationContext;
};



#endif //EVALSTATE_H
