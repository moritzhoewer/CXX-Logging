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

#ifndef LOGGING_LOGGER_H_
#define LOGGING_LOGGER_H_

// uncomment if initializer_list is broken
//#define ENABLE_INITIALIZER_LIST_WORKAROUND

#include "logging/LogRecord.h"
#include <string>
#include <fstream>
#include <map>
#include <vector>

namespace logging {

/**
 * Defines the levels of severity in decreasing order
 */
enum class LogLevel {
    ERROR, WARNING, INFO, DEBUG, TRACE, OFF
};
std::ostream& operator<<(std::ostream& os, const LogLevel& ll);

/**
 * An implementation for a Logger
 *
 * @author  Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 * @version 1.0
 */
class Logger {
private:
    LogLevel defaultCoutLogLevel; ///< default LogLevel for printing to std::cout
    LogLevel defaultFileLogLevel; ///< default LogLevel for printing to the logfile
    std::ofstream file; ///< handle to the logfile
    std::map<std::string, std::tuple<LogLevel, LogLevel>> logLevels; ///< LogLevels per module
    std::vector<std::string> moduleList; ///< white or blacklist of modules, determined by moduleListIsWhitelist
    bool moduleListIsWhitelist; ///< is the module list a whitelist
public:
    static Logger& getLogger(); // Singleton

public:
    /**
     * Delete Copy constructor
     */
    Logger(const Logger&) = delete;

    /**
     * Delete Copy assignment
     */
    Logger& operator=(const Logger&) = delete;

    LogRecord startLog(const LogLevel& logLevel, const std::string& module);
    void log(const std::string& message, const LogLevel& logLevel, const std::string& module);

    void setDefaultCoutLogLevel(LogLevel defaultCoutLogLevel);
    void setDefaultFileLogLevel(LogLevel defaultFileLogLevel);
    void setLogfile(std::string filename);

    void setLogLevelsForModule(const std::string& module, LogLevel coutLogLevel, LogLevel fileLogLevel);

#ifdef ENABLE_INITIALIZER_LIST_WORKAROUND
    template<typename T = std::string, typename... Targs>
    void setModuleWhitelist(std::string module, Targs... modules);
    void setModuleWhitelist();

    template<typename T = std::string, typename... Targs>
    void setModuleBlacklist(std::string module, Targs... modules);
    void setModuleBlacklist();
#else
    void setModuleWhitelist(std::initializer_list<std::string> modules);
    void setModuleBlacklist(std::initializer_list<std::string> modules);
#endif
private:
    Logger();

};

std::string getTimeSinceStart();
const char* getRelativePath(const char *absolutePath);

#ifdef ENABLE_INITIALIZER_LIST_WORKAROUND
/**
 * Recursively add all modules to the module whitelist
 *
 * @param module the module to add
 * @param modules parameter pack that is recursively expanded
 */
template<typename T, typename ... Targs>
inline void logging::Logger::setModuleWhitelist(std::string module,
        Targs ... modules) {
    setModuleWhitelist(modules...);
    moduleList.push_back(module);
}

/**
 * Recursively add all modules to the module blacklist
 *
 * @param module the module to add
 * @param modules parameter pack that is recursively expanded
 */
template<typename T, typename ... Targs>
inline void logging::Logger::setModuleBlacklist(std::string module,
        Targs ... modules) {
    setModuleBlacklist(modules...);
    moduleList.push_back(module);
}
#endif
} /* namespace logging */

#endif /* LOGGING_LOGGER_H_ */
/** @} */
