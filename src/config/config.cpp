#include "config.hpp"

#include <cstdlib>
#include <fstream>
#include <sysexits.h>

#include "../env.hpp"
#include "../logger/logger.hpp"

/**
 * @brief Construct a new Config object
 */
Config::Config() {
    this->v_ok = !this->read();
}

/**
 * @brief Read the JSON config file and save it to configFileData
 *
 * @return \b 0 - No issues occured \n
 * @return \b 1 - Something went wrong
 */
bool Config::read() {
    std::ifstream configStream(this->configPath());
    if (configStream.is_open()) {
        try {
            this->configFileData.update(nlohmann::json::parse(configStream));
            this->fromJSON();
            return 0;
        } catch (nlohmann::json::parse_error err) {
            configStream.close();
            Logger::error("Failed to parse config file while reading.");
            exit(EX_CONFIG);
            return 1;
        }
    } else {
        configStream.close();
        Logger::error("Failed to open config file while reading.");
        exit(EX_CONFIG);
        return 1;
    }
}

/**
 * @brief Write config data from the config and save it to the JSON config file
 *
 * @return \b 0 - No issues occured \n
 * @return \b 1 - Something went wrong
 */
bool Config::write() {
    this->toJSON();
    std::string configSerialized = this->configFileData.dump();
    std::ofstream configStream;
    configStream.open(this->configPath(), std::ios::trunc);
    if (configStream.is_open()) {
        for (char c : configSerialized) {
            configStream.put(c);
        }
        configStream.close();
        return 0;
    } else {
        configStream.close();
        Logger::error("Failed to open config file while writing.");
        return 1;
    }
}

/**
 * @brief Translate from configFileData to class member variables
 */
void Config::fromJSON() {
    // Version
    if (this->configFileData["version"].is_string()) {
        this->version = this->configFileData["version"];
    } else {
        this->version = FUP_VERSION;
        Logger::log("Key \"version\" in config.json must be a valid string. Ignoring and continuing.");
    }
    // Server
    if (this->configFileData["server"].is_string()) {
        this->server = this->configFileData["server"];
    } else {
        Logger::log("Key \"server\" in config.json must be a valid string.");
        Logger::error("Key \"server\" in config.json is required to continue.");
        exit(EX_CONFIG);
    }
    // API Key
    if (this->configFileData["apikey"].is_string()) {
        this->apikey = this->configFileData["apikey"];
    } else {
        Logger::log("Key \"apikey\" in config.json must be a valid string.");
        Logger::error("Key \"apikey\" in config.json is required to continue.");
        exit(EX_CONFIG);
    }
    // Command
    if (this->configFileData["command"].is_string()) {
        this->command = this->configFileData["command"];
    } else {
        Logger::log("Key \"command\" in config.json must be a valid string.");
        Logger::error("Key \"command\" in config.json is required to continue.");
        exit(EX_CONFIG);
    }
}

/**
 * @brief Translate from class member variables to configFileData
 */
void Config::toJSON() {
    this->configFileData["version"] = this->version;
    this->configFileData["server"] = this->server;
    this->configFileData["apikey"] = this->version;
    this->configFileData["command"] = this->command;
}

/**
 * @brief Returns the path to the app's config file
 *
 * @return \b std::string - The path to the config file.
 */
std::string Config::configPath() {
    if (this->v_configPath.empty()) {
        return (std::string)getenv("HOME") + "/.config/flameshot-uploader/config.json";
    } else {
        return this->v_configPath;
    }
}

/**
 * @brief Returns the state of the config.
 *
 * @return \b true - The config is ready to be used. \n
 * @return \b false - Something went wrong.
 */
bool Config::ok() {
    return this->v_ok;
}
