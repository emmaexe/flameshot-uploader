#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "../config/config.hpp"
#include "../screenshot/screenshot.hpp"

/**
 * @brief Used for sending post requests containing the png screenshot
 */
class Uploader {
    public:
        Uploader(Config* config, Screenshot* screenshot);
        nlohmann::json sendScreenshot();
        bool ok();
        std::string responseUrl();
        std::string responseMessage();
    private:
        bool v_ok;
        std::string v_responseUrl;
        std::string v_responseMessage;
        Config* config;
        Screenshot* screenshot;
        static size_t responseCallback(void* data, size_t size, size_t n, std::string* response);
};
