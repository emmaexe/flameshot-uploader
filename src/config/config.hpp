#pragma once

#include <nlohmann/json.hpp>
#include <string>

// TODO: Make the class static

/**
 * @brief Used for managing the configuration of the app.
 */
class Config {
    public:
        Config();
        bool ok();
        bool read(), write();
        std::string configPath();
        // Config entries after this:
        std::string version;
        std::string server;
        std::string apikey;
        std::string command;
    private:
        bool v_ok;
        void fromJSON(), toJSON();
        std::string v_configPath;
        nlohmann::json configFileData;
};
