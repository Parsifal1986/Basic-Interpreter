/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <cinttypes>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"
#include "statement.hpp"

/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

bool keyword_check(std::string token);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

bool keyword_check(std::string token) {
    if (token == "LET" || token == "PRINT" || token == "INPUT" || token == "RUN" || token == "LIST" || token == "CLEAR" ||
        token == "QUIT" || token == "HELP" || token == "REM" || token == "IF" || token == "GOTO" || token == "END") {
        return true;
    }
    return false;
}

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string token = scanner.nextToken();

    if (token == "LET") {
        std::string var = scanner.nextToken();
        if (keyword_check(var)) {
            error("SYNTAX ERROR");
        }
        if (scanner.nextToken() != "=") {
            error("SYNTAX ERROR");
        }
        Expression *exp = parseExp(scanner);
        int value;
        try {
            value = exp->eval(state);
        } catch (ErrorException &ex) {
            delete exp;
            throw ex;
        }
        state.setValue(var, value);
        delete exp;
    } else if (token == "PRINT") {
        Expression *exp = parseExp(scanner);
        int value;
        try {
            value = exp->eval(state);
        } catch (ErrorException &ex) {
            delete exp;
            throw ex;
        }
        std::cout << value << std::endl;
        delete exp;
    } else if (token == "INPUT") {
        std::string var = scanner.nextToken();
        std::cout << " ? ";
        std::string input;
        getline(std::cin, input);
        int value = stringToInt(input);
        state.setValue(var, value);
    } else if (token == "RUN") {
        int lineNumber = program.getFirstLineNumber();
        while (true) {
            if (lineNumber == -1) {
                break;
            }
            Statement *stmt = program.getParsedStatement(lineNumber);
            stmt->execute(state, program, lineNumber);
        }
    } else if (token == "LIST") {
        int lineNumber = program.getFirstLineNumber();
        while (lineNumber != -1) {
            std::cout << program.getSourceLine(lineNumber) << std::endl;
            lineNumber = program.getNextLineNumber(lineNumber);
        }
    } else if (token == "CLEAR") {
        program.clear();
        state.Clear();
    } else if (token == "QUIT") {
        exit(0);
    } else if (token == "HELP") {
        std::cout << "HELP" << std::endl;
    } else {
        if (token.empty()) {
            return;
        }
        if (scanner.getTokenType(token) == NUMBER) {
            int lineNumber = stringToInteger(token);
            if (scanner.hasMoreTokens()) {
                program.addSourceLine(lineNumber, line);
                Statement *stmt;
                std::string token = scanner.nextToken();
                if (token == "REM") {
                    stmt = new RemStmt;
                } else if (token == "IF") {
                    stmt = new IfStmt;
                } else if (token == "LET") {
                    stmt = new LetStmt;
                } else if (token == "PRINT") {
                    stmt = new PrintStmt;
                } else if (token == "INPUT") {
                    stmt = new InputStmt;
                } else if (token == "GOTO") {
                    stmt = new GotoStmt;
                } else if (token == "END") {
                    stmt = new EndStmt;
                }else {
                    error("SYNTAX ERROR");
                }
                stmt->parseStatement(scanner);
                program.setParsedStatement(lineNumber, stmt);
            } else {
                program.removeSourceLine(lineNumber);
            }
        } else {
            error("SYNTAX ERROR");
        }
    }
}

