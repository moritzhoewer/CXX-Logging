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

#ifndef LOGGING_CONFIG_H_
#define LOGGING_CONFIG_H_

/**
 * Configure path to Logger.cpp.
 * Used to calculate offset for correct paths in logging
 */
#define RELATIVE_FILEPATH "src/logging/Logger.cpp"

/*
 * Configure default LogLevels here
 */
#define DEFAULT_LOGLEVEL_FILE 	LogLevel::DEBUG
#define DEFAULT_LOGLEVEL_COUT 	LogLevel::INFO

/*
 * Configure first lien in log file
 */
#define LOGFILE_HEADER 			"Change this in include/logging/config.h"
#define LOGFILE_SHOW_BUILD		1

/*
 * Configure color here
 */
#define USE_COLORS 				1

#endif /* LOGGING_CONFIG_H_ */
/** @} */
