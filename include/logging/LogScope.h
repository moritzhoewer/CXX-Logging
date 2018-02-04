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

#ifndef LOGGING_LOGSCOPE_H_
#define LOGGING_LOGSCOPE_H_

#include <string>

namespace logging {

/**
 * Convenient way to log scopes.
 *
 * Prints file, line and function (line is the line containing LOG_SCOPE;) and
 * also provides unique (and increasing) ID to associate entering and leaving
 * scope messages.
 *
 * @author  Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 * @version 1.0
 */
class LogScope {
private:
    const char *file;   ///< the file where LOG_SCOPE; macro is called
    const char *function;   ///< the function where LOG_SCOPE macro is called
    int line; ///< the line where LOG_SCOPE macro is called
    int id; ///< ID of the scope
    std::string module; ///< the module to be logging to
    static int instanceCounter;
public:
    LogScope(const char* file, int line, const char* function, std::string module);
    ~LogScope();

private:
    void logScope(std::string message);
};

} /* namespace logging */

#endif /* LOGGING_LOGSCOPE_H_ */
/** @} */
