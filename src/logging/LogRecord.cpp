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

#include "logging/LogRecord.h"
#include "logging/Logger.h"

namespace logging {

/**
 * Constructs a LogRecord.
 *
 * @param logger reference to the Logger
 * @param logLevel the LogLevel of this message
 * @param module the module of this message
 */
LogRecord::LogRecord(Logger& logger, const LogLevel& logLevel,
        const std::string& module) :
        std::ostream(this), logger(logger), logLevel(logLevel), module(module) {
    setp(buffer, buffer + BUFFER_SIZE);
}

/**
 * Private Move-Constructor - only has one purpose.
 * We need this, because LogRecord is created and returned by a method in
 * Logger.
 *
 * @attention This Move-constructor is **Incomplete!** - do not use!
 */
LogRecord::LogRecord(LogRecord&& logRecord) :
        logger(logRecord.logger), logLevel(logRecord.logLevel), module(
                logRecord.module) {
}

/**
 * Destructs a LogRecord, syncing
 */
LogRecord::~LogRecord() {
    if (pbase() != pptr()) { // there is still something in the buffer!
        sync();
    }
}

/**
 * Called by the underlying streambuf if the buffer is overflowing.
 * Will pass the buffer (+ the overflowed character) to the Logger and then
 * clear the buffer.
 *
 * @param i the character that overflowed
 */
int LogRecord::overflow(int i) {

    /*
     * convert buffer to std::string.
     * pbase() points to beginning of the buffer, pptr() points to current char
     * The constructor used expects pointer to the beginning of a string and
     * the length of the string
     */
    std::string message(pbase(), pptr() - pbase());

    if (i != std::char_traits<char>::eof()) { // overflowed a char
        message += std::string(1, (char) i);
    }

    logger.log(message, logLevel, module);
    setp(buffer, buffer + BUFFER_SIZE); // reset buffer
    return 0;
}

/**
 * Called by the underlying streambuf when the stream is flushed.
 * Passes the buffer over to Logger and then resets it.
 */
int LogRecord::sync() {
    /*
     * convert buffer to std::string.
     * pbase() points to beginning of the buffer, pptr() points to current char
     * The constructor used expects pointer to the beginning of a string and
     * the length of the string
     */
    logger.log(std::string(pbase(), pptr() - pbase()), logLevel, module);
    setp(buffer, buffer + BUFFER_SIZE); // reset buffer
    return 0;
}

} /* namespace logging */
/** @} */
