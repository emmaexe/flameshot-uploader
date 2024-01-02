#pragma once

#include <string>
#include "../config/config.hpp"

/**
 * @brief Used for taking and storing screenshots. \n Can only store one screenshot at a time.
 */
class Screenshot {
    public:
        Screenshot(Config* config);
        std::string take();
        std::string get();
    private:
        Config* config;
        std::string png;
};
