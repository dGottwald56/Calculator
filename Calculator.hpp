#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "CalculatorInterface.hpp"

class CalculatorNode
{
    CalculatorNode *prev = nullptr; // initialize pointer allowing us to traverse list in reverse
    CalculatorNode *next = nullptr; // initialize pointer allowing us to traverse list forward
    FUNCTIONS nodeOperator;         // data element storing enumerated value of operator to be used in node
    double nodeTotal = 0.0;         // data element storing the total value for the node
    double nodeOperand = 0.0;       // data element storing the operand of the node
    friend class CalcList;          // make CalcList a friend of CalculatorNode, so it may access private values
};

class CalcList : public CalcListInterface
{
public:
    CalcList();                                                    // default constructor
    ~CalcList();                                                   // declaration of destructor
    double total() const;                                          // declaration of function to return current total
    void newOperation(const FUNCTIONS func, const double operand); // declaration of function to add a new node and therefore operation to the CalcList
    void removeLastOperation();                                    // declaration of function to act as an "undo" operation on the CalcList
    void clearOperations();                                        // declaration of function to clear all operations from the CalcList
    std::string toString(unsigned short precision) const;          // declaration of function to return string of operations
    friend class Calculator;
private:
    // sentinel nodes, will hold nothing and be used to access to traverse list in either direction starting from the beginning or the end.
    CalculatorNode *header, *trailer;
    void addNode(CalculatorNode *tNode, const double &data); 
    void removeNode(CalculatorNode *oldNode); 
    bool empty() const { return (header->next == trailer); } 
};
// end definition of CalcList
#endif // CALCULATOR_H