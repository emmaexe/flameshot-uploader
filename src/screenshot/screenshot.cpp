#include "screenshot.hpp"

#include <sysexits.h>

#include "../logger/logger.hpp"
#include "../env.hpp"

/**
 * @brief Construct a new Screenshot object
 *
 * @param config
 */
Screenshot::Screenshot(Config* config) : config(config), png("") {}

/**
 * @brief Starts the flameshot gui and saves the result.
 *
 * @return \b std::string - the raw image data of the screenshot in png format.
 */
std::string Screenshot::take() {
    std::FILE* pipe;
    char charBuf = 'a';
    std::string cmd = this->config->command;

    // Drop flameshot's stderr if debugging is turned off
    #if !FUP_DEBUG
    cmd+=" 2>/dev/null";
    #endif

    if ((pipe = popen(cmd.c_str(), "r")) == 0) {
        Logger::error("popen() failed, could not take screenshot. This may be due to a misconfigured \"command\" in the config file. Aborting.");
        exit(EX_UNAVAILABLE);
    }
    while (fread(&charBuf, sizeof charBuf, 1, pipe)) {
        this->png.push_back(charBuf);
    }
    pclose(pipe);
    return this->png;
}

/**
 * @brief Get the last taken screenshot stored in this object.
 *
 * @return \b std::string - The raw image data of the last taken screenshot in png format.
 */
std::string Screenshot::get() {
    return this->png;
}
