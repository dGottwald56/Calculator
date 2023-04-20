#include "Calculator.hpp"
#include "Command.hpp"


CalcList::CalcList():CalcListInterface() 
{
    header = new CalculatorNode;  
    trailer = new CalculatorNode;
    header->next = trailer;         //set header's next pointer to trailer to create linked list
    trailer->prev = header;         //set trailer's prev pointer to header to complete linked list intitialization
}

CalcList::~CalcList() 
{
    while (!empty()) 
    {                               //if header's next pointer does not point to trailer,
        removeNode(header->next);   //begin at header and call function that removes each subsequent node
    }
        delete header;              //once header's next points to trailer, list is empty, delete nodes to deallocate memory
        delete trailer;
}

//function to add a node for a new operation
void CalcList::addNode(CalculatorNode* tNode, const double& data) 
{
    CalculatorNode* newNode = new CalculatorNode;newNode->nodeTotal = data;     //create new node to store new operations, assign nodeTotal of said node to the input double
    newNode->next = tNode;                                                      //new node's next pointer points to trailer node
    newNode->nodeTotal = tNode->prev->nodeTotal;                                //new node's nodeTotal is assigned the value contained in the node behind the trailer node
    newNode->prev = tNode->prev;                                                //new node's prev pointer points at what the trailer node is currently pointing at
    tNode->prev->next = newNode;                                                //the new node is pointed at by the last node's next pointer
    tNode->prev = newNode;                                                      //what the trailer node's prev pointer points at is updated to the new node    
}

//function to remove a node, necessary for undo command
void CalcList::removeNode(CalculatorNode* oldNode) 
{
    if(empty()) 
    { 
        throw ("Cannot remove operation since no operations have been performed thus far.");
    }     
    
    else 
        {
        CalculatorNode* newLastNode = oldNode->prev;        //create node to link the new node for the trailer node's prev pointer to point at
        CalculatorNode* newTrailerNode = oldNode->next;     //create node for the old node's next pointer to point at
        newLastNode->next = newTrailerNode;                 //link the new last node's next pointer to the trailer node
        newTrailerNode->prev = newLastNode;                 //link the trailer node's prev pointer to the new last node
        delete oldNode;                                     //delete old node
        }
}

//intitialization of function to return current total
double CalcList::total() const
{
    return trailer->prev->nodeTotal;        //access node that trailer's prev pointer points at, return the total stored in that node
}

//intialization of function to add a new node and operation to the list
void CalcList::newOperation(const FUNCTIONS func, const double operand) 
{
    addNode(trailer,0.00);

    //switch statement to determine operator input
    switch (func)
    {
    case ADDITION:                                              //if addition [0]
        trailer->prev->nodeOperator = ADDITION;                 //access last node (node behind trailer) and assign addition operator to nodeOperator
        trailer->prev->nodeOperand = operand;                   //access last node "" and assign input double to the operand
        trailer->prev->nodeTotal += operand;                    //access last node "" and add the input double to the node's total
        break;
    case SUBTRACTION:                                           //if subtraction [1]
        trailer->prev->nodeOperator = SUBTRACTION;              //access last node (node behind trailer) and assign subtraction operator to nodeOperator
        trailer->prev->nodeOperand = operand;                   //access last node "" and assign input double to the operand
        trailer->prev->nodeTotal -= operand;                    //access last node "" and subtract the input double from the node's total
        break;
    case DIVISION:                                              //if division [2]
        if(operand == 0) 
        { 
            throw("Cannot divide by zero.");                    //throw error to catch a divide by zero instance
        }
        else 
        {                                                  
        trailer->prev->nodeOperator = DIVISION;                 //access last node (node behind trailer) and assign division operator to nodeOperator
        trailer->prev->nodeOperand = operand;                   //access last node "" and assign input double to the operand
        trailer->prev->nodeTotal /= operand;                    //access last node "" and divide the node's total by the input double
        break;
        }
    case MULTIPLICATION:                                        //if multiplication [3]
        trailer->prev->nodeOperator = MULTIPLICATION;           //access last node (node behind trailer) and assign multiplication operator to nodeOperator
        trailer->prev->nodeOperand = operand;                   //access last node "" and assign input double to the operand
        trailer->prev->nodeTotal *= operand;                    //access last node "" and multiply the node's total by the input double
        break;
    default:
        throw ("Invalid input, please try again.");             //throw an error to the user if input is not recognized
        break;
    }
}

//function to act as an "undo" command
void CalcList::removeLastOperation() 
{
    removeNode(trailer->prev); 
}

void CalcList::clearOperations() 
{
    while (!empty()) 
    { 
        removeNode(header->next);   
    }
}

//function to display operations completed so far as a string
std::string CalcList::toString(unsigned short precision) const
{
    std::stringstream ss; 
    std::string dataToString = ""; 
    CalculatorNode* tempNode = header; 

    ss.precision(precision);
    
    while(tempNode->next != trailer && tempNode != nullptr)    
    {    
        ss << std::fixed << tempNode->nodeOperand;

        //switch statement to print the operator
        switch (tempNode->next->nodeOperator)
        {
        case ADDITION: 
            ss << " ";
            ss << "+"; 
            ss << " ";
            break;
        case SUBTRACTION: 
            ss << " ";
            ss << "-";
            ss << " ";
            break;
        case DIVISION:
            ss << " ";
            ss << "/"; 
            ss << " ";
            break;
        case MULTIPLICATION:   
            ss << " ";
            ss << "*";  
            ss << " ";
            break;
        }
        tempNode = tempNode->next; 
    }
        if(tempNode != nullptr)
        {
            ss << std::fixed << tempNode->nodeOperand;
        } 

        ss << " ";
        ss << "=";                                              
        ss << " ";
        ss << std::fixed << trailer->prev->nodeTotal; 

    dataToString = ss.str(); 
    return dataToString;
} 

class Calculator {
    public:
        bool doHelp() 
        {
            std::cout << "Commands:" << std::endl;
            std::cout << "help - displays this help message" << std::endl;
            std::cout << "add <number> - adds the number to the total" << std::endl;
            std::cout << "subtract <number> - subtracts the number from the total" << std::endl;
            std::cout << "multiply <number> - multiplies the number by the total" << std::endl;
            std::cout << "divide <number> - divides the total by the number" << std::endl;
            std::cout << "undo - removes the last operation from the total" << std::endl;
            std::cout << "clear - removes all operations from the total" << std::endl;
            std::cout << "print - displays operations performed so far" << std::endl;
            std::cout << "quit - exits the program" << std::endl;
            return true;
        }

        void run() {
            CommandParser parser;
            CalcList calc;

            std::cout << "Welcome to the calculator program." << std::endl;
            std::cout << "Please enter a command or enter 'help' for a list of commands." << std::endl;
 
            bool shouldContinue = true;
            while (shouldContinue)
            {
                std::cout << "> ";
                Command cmd = parser.fetchCommand();

            if (cmd.getCommand().compare("help") == 0) 
                {
                    doHelp();
                }
            else if (cmd.getCommand().compare("add") == 0)
                {
                    std::vector<std::string> num = cmd.getParameters();
                    calc.newOperation(ADDITION, std::stod(num[0]));
                }
            else if (cmd.getCommand().compare("subtract") == 0)
                {
                    std::vector<std::string> num = cmd.getParameters();
                    calc.newOperation(SUBTRACTION, std::stod(num[0]));
                }
            else if (cmd.getCommand().compare("multiply") == 0)
                {
                    std::vector<std::string> num = cmd.getParameters();
                    calc.newOperation(MULTIPLICATION, std::stod(num[0]));
                }
            else if (cmd.getCommand().compare("divide") == 0)
                {
                    std::vector<std::string> num = cmd.getParameters();
                    calc.newOperation(DIVISION, std::stod(num[0]));
                }
            else if (cmd.getCommand().compare("undo") == 0)
                {
                    calc.removeLastOperation();
                }
            else if (cmd.getCommand().compare("clear") == 0)
                {
                    calc.clearOperations();
                }
            else if (cmd.getCommand().compare("print") == 0)
                {
                    std::string output = calc.toString(3);
                    std::cout << output << std::endl;
                }
            else if (cmd.getCommand().compare("quit") == 0)
                {
                    std::cout << "Goodbye!" << std::endl;
                    break;
                }
            else if (cmd.getCommand().compare("") == 0)
                {
                    // do nothing
                }
            else
                {
                    std::cout << "Invalid command. Please enter a valid command or enter 'help' for a list of commands." << std::endl;
                }
            }  
        }
    };

int main() {
    Calculator calculator;
    calculator.run();
}
