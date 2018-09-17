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

#include "logging/Logger.h"
#include "logging/config.h"
#include <chrono>
#include <algorithm>
#include <cstring>
#include <cassert>

using namespace std::chrono;

namespace logging {

/**
 * Constructs a Logger
 */
Logger::Logger() :
        defaultCoutLogLevel(DEFAULT_LOGLEVEL_COUT), defaultFileLogLevel(
                DEFAULT_LOGLEVEL_FILE), file(), moduleListIsWhitelist(false) {
}

/**
 * Singleton implementation.
 */
Logger& Logger::getLogger() {
    static Logger instance;
    return instance;
}

/**
 * Changes the default LogLevel for std::cout
 */
void Logger::setDefaultCoutLogLevel(LogLevel defaultCoutLogLevel) {
    this->defaultCoutLogLevel = defaultCoutLogLevel;
}

/**
 * Changes the default LogLevel for the logfile
 */
void Logger::setDefaultFileLogLevel(LogLevel defaultFileLogLevel) {
    this->defaultFileLogLevel = defaultFileLogLevel;

}

/**
 * Sets/Changes the logfile.
 * Closes the existing logfile (if one such file exists) and then opens the
 * logfile specified by filename, overwriting it if it exists already
 */
void Logger::setLogfile(std::string filename) {
    file.close();
    file.open(filename, std::ios::trunc);
    file << LOGFILE_HEADER;
#if LOGFILE_SHOW_BUILD
	file <<	" - Build: " << __DATE__ << ", " << __TIME__;
#endif
	file << std::endl;
}

/**
 * Create a LogRecord object to start a new log message
 *
 * @param logLevel the LogLevel to pass to the LogRecord object.
 * @param module the Module to pass to the
 * @return a LogRecord object
 */
LogRecord Logger::startLog(const LogLevel& logLevel,
        const std::string& module) {
    return LogRecord(*this, logLevel, module);
}

/**
 * Log a message.
 * Depending on the flags, message is printed to std::cout and/or logfile
 *
 * @param message the message to log
 * @param logLevel the LogLevel
 * @param module the module
 */
void Logger::log(const std::string& message, const LogLevel& logLevel,
        const std::string& module) {
    bool logCout;
    bool logFile;

    // check if module is in the list
    bool inList = std::find(moduleList.begin(), moduleList.end(), module)
            != moduleList.end();

    if (moduleListIsWhitelist == inList) {
        // whitelist and in list, or blacklist and not in list

        LogLevel coutLogLevel = defaultCoutLogLevel;
        LogLevel fileLogLevel = defaultFileLogLevel;

        if (logLevels.find(module) != logLevels.end()) {
            // specific log levels are set
            std::tie(coutLogLevel, fileLogLevel) = logLevels[module];
        }

        logCout = static_cast<int>(logLevel) <= static_cast<int>(coutLogLevel);
        logFile = static_cast<int>(logLevel) <= static_cast<int>(fileLogLevel);

    } else {
        logCout = false;
        logFile = false;
    }

    if (logCout) {
        std::cout << message << std::flush;
    }
    if (logFile) {
        file << message << std::flush;
    }
}

/**
 * Sets custom LogLevels for a specific module.
 *
 * @param module       the module to set the LogLevels for
 * @param coutLogLevel the LogLevel to use for cout
 * @param fileLogLevel the LogLevel to use for logging to a file
 */
void Logger::setLogLevelsForModule(const std::string& module,
        LogLevel coutLogLevel, LogLevel fileLogLevel) {
    logLevels[module] = std::make_tuple(coutLogLevel, fileLogLevel);
}

#ifdef ENABLE_INITIALIZER_LIST_WORKAROUND
/**
 * Ends the recursion for adding modules to whitelist.
 * Gets called first and therefore prepares for the push_backs() that follow.
 */
void Logger::setModuleWhitelist() {
    moduleListIsWhitelist = true;
    moduleList.clear();
}

/**
 * Ends the recursion for adding modules to blacklist.
 * Gets called first and therefore prepares for the push_backs() that follow.
 */
void Logger::setModuleBlacklist() {
    moduleListIsWhitelist = false;
    moduleList.clear();
}
#else
/**
 * Sets a module Whitelist.
 * Logging will only be shown for the modules on this list.
 *
 * @attention setModuleWhitelist and setModuleBlackist are mutually exclusive!
 *
 * @param modules the list of modules to whitelist
 */
void Logger::setModuleWhitelist(std::initializer_list<std::string> modules) {
    moduleListIsWhitelist = true;
    moduleList = modules;
}

/**
 * Sets a module Blacklist.
 * Logging will not be shown for the modules on this list.
 *
 * @attention setModuleWhitelist and setModuleBlackist are mutually exclusive!
 *
 * @param modules the list of modules to blacklist
 */
void Logger::setModuleBlacklist(std::initializer_list<std::string> modules) {
    moduleListIsWhitelist = false;
    moduleList = modules;
}
#endif

/**
 * start time of the program (as base for time later)
 */
static auto startTime = steady_clock::now();

/**
 * get the current ms as time since program started
 *
 * @return time since program started in ms
 */
static int getCurrentMS() {
    return duration_cast<milliseconds>(steady_clock::now() - startTime).count();
}

/**
 * pad the a number with 0
 *
 * @param number the number to pad
 * @param digits the number of digits
 * @return a string with the padded number
 */
static std::string padNumber(int number, int digits) {
    std::string result = std::to_string(number);

    int ref = 1;

    for (digits--; digits > 0; digits--) {
        ref *= 10;
        if (number < ref) {
            result = "0" + result;
        }
    }
    return result;
}

/**
 * Returns time since program start.
 * the format is mm:ss.msmsms (eg. 00:00.310 / 02:10.000)
 */
std::string getTimeSinceStart() {
    int ms = getCurrentMS();
    int secs = ms / 1000;
    int mins = secs / 60;
    secs = secs % 60;
    ms = ms % 1000;

    return padNumber(mins, 2) + ":" + padNumber(secs, 2) + "."
            + padNumber(ms, 3);
}

/**
 * @param absolutePath the absolute path to reduce
 * @return relative path to src/ from absolute file path
 */
const char* getRelativePath(const char* absolutePath) {
    static const int offset = (int)std::strlen(__FILE__)
            - std::strlen(RELATIVE_FILEPATH);
//    static_assert(offset >= 0, "Negative offset!");
    assert(offset >= 0);
    assert((int)std::strlen(absolutePath) > offset);
    return absolutePath + offset;
}

/**
 * Print the LogLevel to an ostream.
 *
 * @param os the std::ostream to print to
 * @param ll the LogLevel to print
 *
 * @return os
 */
std::ostream& operator <<(std::ostream& os, const LogLevel& ll) {
    switch (ll) {
    case LogLevel::ERROR:
        os << "ERROR";
        break;
    case LogLevel::WARNING:
        os << "WARNING";
        break;
    case LogLevel::INFO:
        os << "INFO";
        break;
    case LogLevel::DEBUG:
        os << "DEBUG";
        break;
    case LogLevel::TRACE:
        os << "TRACE";
        break;
    case LogLevel::OFF:
        os << "OFF";
        break;
    }
    return os;
}

} /* namespace logging */
/** @} */
