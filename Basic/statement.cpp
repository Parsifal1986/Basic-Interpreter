/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "Utils/tokenScanner.hpp"
#include "parser.hpp"
#include <cstdio>
#include <string>


/* Implementation of the Statement class */

bool keyword_check(std::string token);

Statement::Statement() = default;

Statement::~Statement() = default;

void Statement::execute(EvalState &state, Program &program, int &lineNumber) {
  return;
}

void Statement::parseStatement(TokenScanner &scanner) {
    return;
}

void RemStmt::execute(EvalState &state, Program &program, int &lineNumber) {
    lineNumber = program.getNextLineNumber(lineNumber);
    return;
}

void RemStmt::parseStatement(TokenScanner &scanner) {
    return;
}

void LetStmt::execute(EvalState &state, Program &program, int &lineNumber) {
    if (keyword_check(var)) {
        error("SYNTAX ERROR");
    }
    int value = exp->eval(state);
    state.setValue(var, value);
    //delete exp;
    lineNumber = program.getNextLineNumber(lineNumber);
    return;
}

void LetStmt::parseStatement(TokenScanner &scanner) {
    var = scanner.nextToken();
    if (scanner.nextToken() != "=") {
        error("SYNTAX ERROR");
    }
    exp = parseExp(scanner);
  return;
}

LetStmt::~LetStmt() {
    delete exp;
    return;
}

void PrintStmt::execute(EvalState &state, Program &program, int &lineNumber) {
    int value = exp->eval(state);
    std::cout << value << std::endl;
    //delete exp;
    lineNumber = program.getNextLineNumber(lineNumber);
    return;
}

void PrintStmt::parseStatement(TokenScanner &scanner) {
    exp = parseExp(scanner);
    return;
}

PrintStmt::~PrintStmt() {
    delete exp;
    return;
}

void InputStmt::execute(EvalState &state, Program &program, int &lineNumber) {
    std::cout << " ? ";
    std::string input;
    getline(std::cin, input);
    state.setValue(var, stringToInt(input));
    lineNumber = program.getNextLineNumber(lineNumber);
    return;
}

void InputStmt::parseStatement(TokenScanner &scanner) {
    var = scanner.nextToken();
    return;
}

void EndStmt::execute(EvalState &state, Program &program, int &lineNumber) {
    lineNumber = -1;
    return;
}

void EndStmt::parseStatement(TokenScanner &scanner) {
    return;
}

void GotoStmt::execute(EvalState &state, Program &program, int &lineNumber) {
    if (program.getParsedStatement(this->lineNumber) == nullptr) {
        error("LINE NUMBER ERROR");
    }
    lineNumber = this->lineNumber;
    return;
}

void GotoStmt::parseStatement(TokenScanner &scanner) {
    lineNumber = stringToInt(scanner.nextToken());
    return;
}

void IfStmt::execute(EvalState &state, Program &program, int &lineNumber) {
    if (program.getParsedStatement(this->lineNumber) == nullptr) {
        error("LINE NUMBER ERROR");
    }
    switch (op) {
        case '=':
            if (lhs->eval(state) == rhs->eval(state)) {
                lineNumber = this->lineNumber;
            } else {
                lineNumber = program.getNextLineNumber(lineNumber);
            }
            break;
        case '<':
            if (lhs->eval(state) < rhs->eval(state)) {
                lineNumber = this->lineNumber;
            } else {
                lineNumber = program.getNextLineNumber(lineNumber);
            }
            break;
        case '>':
            if (lhs->eval(state) > rhs->eval(state)) {
                lineNumber = this->lineNumber;
            } else {
                lineNumber = program.getNextLineNumber(lineNumber);
            }
            break;
        default:
            error("SYNTAX ERROR");
    }
    if (program.getParsedStatement(lineNumber) == nullptr) {
        error("LINE NUMBER ERROR");
    }
    return;
}

void IfStmt::parseStatement(TokenScanner &scanner) {
    std::string left, right;
    TokenScanner temp_scanner;
    do {
        left += scanner.nextToken();
    } while (left.back() != '<' && left.back() != '>' && left.back() != '=');
    op = left.back();
    left.pop_back();
    temp_scanner.setInput(left);
    lhs = parseExp((temp_scanner));
    std::string temp;
    do {
        if (scanner.hasMoreTokens() == false) {
            error("SYNTAX ERROR");
        }
        temp = scanner.nextToken();
        if (temp == "THEN") {
            break;
        }
        right += temp;
    } while (true);
    temp_scanner.setInput(right);
    rhs = parseExp(temp_scanner);
    if (temp != "THEN") {
        error("SYNTAX ERROR");
    }
    lineNumber = stringToInt(scanner.nextToken());
    return;
}

IfStmt::~IfStmt() {
    delete lhs;
    delete rhs;
    return;
}

int stringToInt(std::string str) {
    std::istringstream stream(str);
    int value;
    stream >> value;
    if (!stream.eof())
        stream >> std::ws;
    if (stream.fail() || !stream.eof()) {
        std::cout << "INVALID NUMBER" << std::endl << " ? ";
        getline(std::cin, str);
        return stringToInt(str);
    }
    return value;
}

//todo
