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

#ifndef LOGGING_LOGRECORD_H_
#define LOGGING_LOGRECORD_H_

#include <iostream>

namespace logging {

// forward declarations
class Logger;
enum class LogLevel;

/**
 * Size of the internal buffer.
 *
 * Should be sufficient for most log messages to prevent interleaved output
 * when multiple threads are printing.
 */
constexpr int BUFFER_SIZE = 256;

/**
 * Convenient interface to the Logger.
 *
 * Inherits from std::ostream so you can use it like you would use std::cout.
 * Buffers the log messages before handing them over to the Logger and keeps
 * track of the LogLevel for the current message.
 *
 * @author  Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 * @version 1.0
 */
class LogRecord : public std::ostream, public std::streambuf {
private:
    Logger& logger; ///< reference to the Logger
    const LogLevel& logLevel; ///< LogLevel of the current message
    std::string module; ///< the module of the current message
    char buffer[BUFFER_SIZE]; ///< buffer for the message
public:
    LogRecord(Logger& logger, const LogLevel& logLevel, const std::string& module);
    ~LogRecord();

    virtual int overflow(int ch) override;
    virtual int sync() override;

    friend class Logger;
private:
    LogRecord(LogRecord&& logRecord);
};

} /* namespace logging */

#endif /* INCLUDE_LOGGING_LOGRECORD_H_ */
/** @} */
