#include "logger.hpp"

#include "../env.hpp"

#define quote(x) #x

#include <iostream>

/**
 * @brief Send an error to stderr
 *
 * @param message
 */
void Logger::error(std::string message) {
    #if FUP_ERRORS
    std::cerr << "[ERROR] " << message << "\n";
    #endif
}

/**
 * @brief Send a log to stderr
 *
 * @param message
 */
void Logger::log(std::string message) {
    #if FUP_LOGS
    std::cerr << "[LOG] " << message << "\n";
    #endif
}

/**
 * @brief Send a debug message to stderr
 *
 * @param message
 */
void Logger::debug(std::string message) {
    #if FUP_DEBUG
    std::cerr << "[DEBUG] " << message << "\n";
    #endif
}
