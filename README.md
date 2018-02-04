# Logging Framework - v1.0
Short usage guide to the logging framework (v1.0) by Moritz Höwer

## Usage
To use logging framework, include `util/logging/logging.h`.  
To print a log message use the provided macros `LOG_XXX` like you would use `std::cout` (XXX is ERROR, WARNING, DEBUG or TRACE). 

### Set logfile
To set or change the logfile use the macro `SET_LOGFILE()` and pass it the filename.

### Set default LogLevel
To set the default LogLevel for console output (std:cout) use macro `SET_LOGLEVEL_COUT()` and pass it a LogLevel (ERROR, WARNING, DEBUG, TRACE, OFF).  
To set the default LogLevel for logfile use macro `SET_LOGLEVEL_FILE()`.

std::cout / the logfile will contain all log messages with a LogLevel that is at least as severe as the one specified. `LogLevel::ERROR` will only print ERROR, `LogLevel::WARNING` will print WARNING and ERROR and so on. 

## Modules
With proper use of modules, we can say more specifically which log messages we want to see.

### Set Module
For adding a file (or to be more precise for a compilation unit) to a module, `#define LOG_MODULE <name>` **before** you `#include "util/logging/logging.h`.  
This will put all following log messages into the module. If no module is specified, log messages are put into the global module.

### Select which modules to display
Using `LOGGING_SET_WHITELIST(...)` or `LOGGING_SET_BLACKLIST(...)`, you can whitelist/blacklist specific (comma seperated) modules.

### Specify custom LogLevels for a module
Using `SET_LOGLEVELS_MODULE(<module>, <LogLevel cout>, <LogLevel file>` you can set specific LogLevels for a module.

## Example
```
#include "util/logging/logging.h"

int main() {
	SET_LOGFILE("output.log");

	SET_LOGLEVEL_COUT(LogLevel::TRACE);
	SET_LOGLEVEL_FILE(LogLevel::WARNING);
	
	LOG_SCOPE
	
	LOG_ERROR << "Test error " << 2 << std::endl;
	LOG_WARNING << "Test warning " << 3.5 << std::endl;
	LOG_DEBUG << "Test debug " << to_string(false) << std::endl;
	LOG_TRACE << "Test trace" << std::endl;
}
```
will print something like

```
[2017-10-15 16:04:29][ TRACE ][GLOBAL][main.cpp:9 (main)]: Entering Scope (0)
[2017-10-15 16:04:29][ ERROR ][GLOBAL][main.cpp:11 (main)]: Test error 2
[2017-10-15 16:04:29][WARNING][GLOBAL][main.cpp:12 (main)]: Test warning 3.5
[2017-10-15 16:04:29][ DEBUG ][GLOBAL][main.cpp:13 (main)]: Test debug false
[2017-10-15 16:04:29][ TRACE ][GLOBAL][main.cpp:14 (main)]: Test trace
[2017-10-15 16:04:29][ TRACE ][GLOBAL][main.cpp:9 (main)]: Leaving Scope (0)
```
and `output.log` will contain something like

```
[2017-10-15 16:04:29][ ERROR ][GLOBAL][main.cpp:11 (main)]: Test error 2
[2017-10-15 16:04:29][WARNING][GLOBAL][main.cpp:12 (main)]: Test warning 3.5
```

