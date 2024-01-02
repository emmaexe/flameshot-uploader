#include <nlohmann/json.hpp>

#include "./config/config.hpp"
#include "./logger/logger.hpp"
#include "./screenshot/screenshot.hpp"
#include "./uploader/uploader.hpp"
#include "../libs/clip/clip.h"

int main(int argc, char *argv[]) {
    Config config;
    Screenshot screenshot(&config);
    if (screenshot.take().empty()) {
        Logger::log("Screenshot aborted.");
        return 0;
    }
    Uploader uploader(&config, &screenshot);
    uploader.sendScreenshot();
    if (uploader.ok()) {
        if (!( clip::set_text(uploader.responseUrl()) )) {
            Logger::error("Unable to copy to clipboard. If you are on wayland, this functionality is not supported.");
            Logger::log("Url: " + uploader.responseUrl());
        }
    } else {
        Logger::error("Uploader failed: " + uploader.responseMessage());
    }
    return 0;
}
