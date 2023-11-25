/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "statement.hpp"
#include <cstddef>



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    while (!lines.empty()) {
        removeSourceLine(*lineNumbers.begin());
    }
    return;
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    if (lineNumbers.find(lineNumber) != lineNumbers.end()) {
        removeSourceLine(lineNumber);
    }
    if (line.empty()) {
        return;
    }
    lineNumbers.insert(lineNumber);
    lines[lineNumber].sourceLine = line;
    return;
}

void Program::removeSourceLine(int lineNumber) {
    auto it = lineNumbers.find(lineNumber);
    if (it == lineNumbers.end()) {
        return;
    }
    auto temp_it = lines.find(lineNumber);
    delete temp_it->second.stmt;
    lines.erase(lineNumber);
    lineNumbers.erase(it);
    return;
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = lineNumbers.find(lineNumber);
    if (it == lineNumbers.end()) {
        return "";
    }
    return lines[lineNumber].sourceLine;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    auto it = lineNumbers.find(lineNumber);
    if (it == lineNumbers.end()) {
        error("SYNTAX ERROR");
    }
    auto temp_it = lines.find(lineNumber);
    delete temp_it->second.stmt;
    temp_it->second.stmt = stmt;
    return;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = lineNumbers.find(lineNumber);
    if (it == lineNumbers.end()) {
        return nullptr;
    }
    auto temp_it = lines.find(lineNumber);
    if (temp_it->second.stmt == nullptr) {
        return NULL;
    }
    return temp_it->second.stmt;
}

int Program::getFirstLineNumber() {
    return lines.empty() ? -1 : *lineNumbers.begin();
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = lineNumbers.upper_bound(lineNumber);
    if (it == lineNumbers.end()) {
        return -1;
    }
    return *it;
}

//more func to add
//todo


