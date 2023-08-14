#ifndef __XLOG_H__
#define __XLOG_H__

#include <iostream>


namespace xLog {
	void error(std::string message, int error_code = -1) {
		std::cerr << "error: " << message << "\n";
		exit(error_code);
	}

	void info(std::string message) {
		std::cout << "log: " << message << "\n";
	}
}

#endif
