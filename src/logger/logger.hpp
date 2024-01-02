#pragma once

#include <string>

/**
 * @brief Used for logging.
 */
class Logger {
    public:
        static void error(std::string message);
        static void log(std::string message);
        static void debug(std::string message);
};
