/*
 * Copyright © 2017-2018 Steven Beyermann, Markus Blechschmidt, Moritz Höwer,
 * Lasse Lüder, Andre Radtke
 *
 * This software is licensed by MIT License.
 * See LICENSE for details.
 */
/** 
 * @file
 */

#ifndef LOGGING_LOGGING_H_
#define LOGGING_LOGGING_H_

#include "logging/LogScope.h"
#include "logging/LogRecord.h"
#include "logging/Logger.h"

using namespace logging;

/**
 * Name of the global module
 */
#define GLOBAL_MODULE "GLOBAL"


// No Module = Global Module
#ifndef LOG_MODULE
    #define LOG_MODULE GLOBAL_MODULE
#endif

// Print the location [File:line (Function)]
#define LOGMESSAGE_LOCATION "[" << getRelativePath(__FILE__) << ":" << __LINE__ << " (" << __FUNCTION__ << ")]: "

// Get a LogRecord (get the Logger, create a LogRecord, set log level)
#define GET_LOG_RECORD(LEVEL, MODULE) \
	Logger::getLogger().startLog(LogLevel::LEVEL, MODULE)

// Prepare a log (Get a LogRecord, print time and print location)
#define PREPARE_LOG(LEVEL, MESSAGE) \
     GET_LOG_RECORD(LEVEL, LOG_MODULE) << "[" << getTimeSinceStart() << "]" << MESSAGE << "[" << LOG_MODULE << "]" << LOGMESSAGE_LOCATION


#define PRINT_SCOPED_LOG(MODULE) \
    GET_LOG_RECORD(TRACE, (MODULE)) << "[" << getTimeSinceStart() << "]" << "[ TRACE ]" << "[" << (MODULE) << "]"

/**
 *  \addtogroup Logging
 * @{
 */

// Prepare log for the different LogLevels
/**
 * Logs an error message
 */
#define LOG_ERROR \
    PREPARE_LOG(ERROR, "[ ERROR ]")

/**
 * Logs a warning
 */
#define LOG_WARNING \
		PREPARE_LOG(WARNING, "[WARNING]")

/**
 * Logs an info message
 */
#define LOG_INFO \
    PREPARE_LOG(INFO, "[  INFO ]")

/**
 * Logs a debug message
 */
#define LOG_DEBUG \
    PREPARE_LOG(DEBUG, "[ DEBUG ]")

/**
 * Logs a tracing message
 */
#define LOG_TRACE \
    PREPARE_LOG(TRACE, "[ TRACE ]")

// Wrappers for logging scope
/**
 * Logs the current scope
 */
#define LOG_SCOPE \
	LogScope logscope(getRelativePath(__FILE__), __LINE__, __FUNCTION__, LOG_MODULE);

// Wrappers to easily set LogLevel and log file
/**
 * Globally sets LogLevel for std::cout
 */
#define SET_LOGLEVEL_COUT(logLevel) \
    Logger::getLogger().setDefaultCoutLogLevel(logLevel)

/**
 * Globally sets LogLevel for logfile
 */
#define SET_LOGLEVEL_FILE(logLevel) \
    Logger::getLogger().setDefaultFileLogLevel(logLevel)

/**
 * Globally sets/changes logfile
 */
#define SET_LOGFILE(filename) \
    Logger::getLogger().setLogfile(filename)


// Wrappers for configuring modules
/**
 * Sets LogLevels for a specific module
 */
#define SET_LOGLEVELS_MODULE(MODULE, COUT, FILE) \
    Logger::getLogger().setLogLevelsForModule(MODULE, COUT, FILE)


#ifdef ENABLE_INITIALIZER_LIST_WORKAROUND
/**
 * Whitelist specific modules
 */
#define LOGGING_SET_WHITELIST(...) \
    Logger::getLogger().setModuleWhitelist(__VA_ARGS__)

/**
 * Blacklist specific modules
 */
#define LOGGING_SET_BLACKLIST(...) \
    Logger::getLogger().setModuleBlacklist(__VA_ARGS__)
#else
/**
 * Whitelist specific modules
 */
#define LOGGING_SET_WHITELIST(...) \
    Logger::getLogger().setModuleWhitelist({__VA_ARGS__})

/**
 * Blacklist specific modules
 */
#define LOGGING_SET_BLACKLIST(...) \
    Logger::getLogger().setModuleBlacklist({__VA_ARGS__})
#endif

#endif /* LOGGING_LOGGING_H_ */
/** @} */
