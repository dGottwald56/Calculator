// #include <iostream>
// #include <string>
// #include <vector>

// class Command {
// public:

//     Command(std::string command, std::vector<std::string> parameters) 
//     : command(command), parameters(parameters) {}

//     std::string getCommand() {
//         return command;
//     }

//     std::vector<std::string> getParameters() {
//         return parameters;
//     }

//     std::string toString() {
//         std::string result = "Command: " + command + "\n";
//         unsigned long size = parameters.size();
//         int sizeConversion = (int) size;
//         for (int i = 0; i < sizeConversion; i++) {
//             result += "parameters[";
//             result += std::to_string(i);
//             result += "] = ";
//             result += parameters[i];
//             result += "\n";
//         }
//         return result;
//     }
//     friend class CommandParser;
//     friend class Calculator;
// private:
//     std::string command;
//     std::vector<std::string> parameters;
// };

// class CommandParser {
// public:
//     Command fetchCommand() {
//         std::string s;
//         std::getline(std::cin, s);
//         if (s.empty()) {
//             return Command("", std::vector<std::string>());
//         }

//         std::vector<std::string> pieces = split(s, "\\s+", 10);
//         std::vector<std::string> params(pieces.begin() + 1, pieces.end());

//         return Command(pieces[0], params);
//     }
//     friend class Command;
//     friend class Calculator;
// private:
//     std::vector<std::string> split(std::string str, std::string delimiter, int maxsplit) {
//         std::vector<std::string> result;
//         size_t pos = 0;
//         int count = 0;
//         while (count < maxsplit && (pos = str.find(delimiter)) != std::string::npos) {
//             result.push_back(str.substr(0, pos));
//             str.erase(0, pos + delimiter.length());
//             count++;
//         }
//         result.push_back(str);
//         return result;
//     }
// };
#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>

class Command {
public:
    Command(std::string command, std::vector<std::string> parameters)
    : command(command), parameters(parameters) {}

    std::string getCommand() {
        return command;
    }

    std::vector<std::string> getParameters() {
        return parameters;
    }

    std::string toString() {
        std::string result = "Command: " + command + "\n";
        int size = parameters.size();
        for (int i = 0; i < size; i++) {
            result += "parameters[";
            result += std::to_string(i);
            result += "] = ";
            result += parameters[i];
            result += "\n";
        }
        return result;
    }
    friend class CommandParser;
private:
    std::string command;
    std::vector<std::string> parameters;
};

class CommandParser {
public:
    Command fetchCommand() {
        std::string s;
        std::getline(std::cin, s);
        if (s.empty()) {
            return Command("", std::vector<std::string>());
        }

        std::vector<std::string> pieces = split(s, "\\s+", -1);
        std::vector<std::string> params(pieces.begin() + 1, pieces.end());

        return Command(pieces[0], params);
    }
    friend class Command;
private:
    std::vector<std::string> split(std::string str, std::string delimiter, int maxsplit) {
        std::vector<std::string> result;
        std::regex re(delimiter);
        std::sregex_token_iterator iter(str.begin(), str.end(), re, -1);
        std::sregex_token_iterator end;

        int count = 0;
        while (iter != end && (maxsplit < 0 || count < maxsplit)) {
            result.push_back(*iter);
            ++iter;
            ++count;
        }

        return result;
    }
};
#endif /* COMMAND_H */