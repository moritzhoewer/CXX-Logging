/*
 * Copyright © 2017-2018 Steven Beyermann, Markus Blechschmidt, Moritz Höwer,
 * Lasse Lüder, Andre Radtke
 *
 * This software is licensed by MIT License.
 * See LICENSE for details.
 */
/** 
 * @file
 * \addtogroup Logging
 * @{
 */

#include "logging/LogScope.h"
#include "logging/logging.h"

namespace logging {

/**
 * Instance counter used to provide unique ID to every scope that's loged
 */
int LogScope::instanceCounter = 0;

/**
 * Log entering a scope.
 * Unique ID is not 100% thread-safe, but it's no desaster if two scopes happen
 * to get the same ID by bad luck.
 */
LogScope::LogScope(const char* file, int line, const char* function, std::string module) :
        file(file), function(function), line(line), id(instanceCounter++), module(module) {
    logScope("Entering Scope");

}

/**
 * Log leaving of scope
 */
LogScope::~LogScope() {
    logScope("Leaving Scope");
}

/**
 * Print the log message for the scope.
 * File, line and function need to be set "manually" here, because using the
 * macros would print [LogScope.cpp:51 (logScope)] every time.
 * Also appends the scope's ID.
 *
 * @param message the message to print
 */
void LogScope::logScope(std::string message) {
    PRINT_SCOPED_LOG(module) <<"[" << file << ":" << line << " (" << function << ")]: "
            << message << " (" << id << ")" << std::endl;
}

}
/* namespace logging */
/** @} */
