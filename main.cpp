#define LOG_MODULE "main"
#include "logging/logging.h"

#include <thread>
#include <chrono>

int main() {
	SET_LOGLEVELS_MODULE("main", LogLevel::TRACE, LogLevel::TRACE);
	SET_LOGFILE("output.log");
	
	LOG_SCOPE;
	LOG_TRACE << "Message 1" << std::endl;
	LOG_DEBUG<< "Message 2" << std::endl;
	LOG_INFO << "Message 3" << std::endl;
	LOG_WARNING << "Message 4" << std::endl;
	LOG_ERROR << "Message 5" << std::endl;
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	LOG_INFO << "Done sleeping..." << std::endl;
}