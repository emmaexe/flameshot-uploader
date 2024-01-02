#include "uploader.hpp"

#include <sysexits.h>

#include <cstdio>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <fstream>

#include "../env.hpp"
#include "../logger/logger.hpp"

/**
 * @brief Construct a new Uploader object
 *
 * @param config the config that the uploader should use
 * @param screenshot the screenshot the uploader should upload
 */
Uploader::Uploader(Config* config, Screenshot* screenshot) : config(config), screenshot(screenshot), v_ok(false), v_responseMessage(""), v_responseUrl("") {}

/**
 * @brief Upload a screenshot to a server according to the config.
 */
nlohmann::json Uploader::sendScreenshot() {
    std::string tmppath = "/tmp/screenshot.png";  // TODO: Revamp to a changable location and check env vars.
    std::ofstream imagefile(tmppath);
    if (imagefile.is_open()) {
        imagefile << this->screenshot->get();
        imagefile.close();
        try {
            std::string responseString = "";
            curlpp::Cleanup cleaner;
            curlpp::Easy request;
            request.setOpt(new curlpp::options::Url(this->config->server));
            request.setOpt(new curlpp::options::Post(true));
            request.setOpt(new curlpp::options::WriteFunction(std::bind(&this->responseCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, &responseString)));

            std::list<std::string> header;
            header.push_back("User-Agent: flameshot-uploader/" FUP_VERSION);
            header.push_back("Content-Type: multipart/form-data");
            header.push_back("x-api-key: " + this->config->apikey);
            request.setOpt(new curlpp::options::HttpHeader(header));

            curlpp::Forms form;
            form.push_back(new curlpp::FormParts::File("attachment", tmppath));
            request.setOpt(new curlpp::options::HttpPost(form));

            request.perform();

            Logger::debug("Request response: " + responseString);

            nlohmann::json responseJSON = nlohmann::json::parse(responseString);
            if (responseJSON["url"].is_string()) {
                this->v_ok = true;
                this->v_responseUrl = responseJSON["url"];
            } else {
                this->v_ok = false;
                this->v_responseMessage = responseJSON["message"];
            }
            return responseJSON;
        } catch (curlpp::LogicError& e) {
            Logger::error(e.what());
        } catch (curlpp::RuntimeError& e) {
            Logger::error(e.what());
        }
        std::remove(tmppath.c_str());
    } else {
        Logger::error("Unable to create temporary file to store screenshot.");
        exit(EX_CANTCREAT); // Not an error on a user specified output file yet, but will be once tmppath is revamped.
    }
    return nlohmann::json();
}

/**
 * @brief Check if the upload was successful
 *
 * @return \b true - Upload succeeded \n
 * @return \b false - Upload failed
 */
bool Uploader::ok() {
    return this->v_ok;
}

/**
 * @brief Returns the url to the screenshot.
 *
 * @return std::string url
 */
std::string Uploader::responseUrl() {
    return this->v_responseUrl;
}

/**
 * @brief Returns the server's response to the request (only if there was an error).
 *
 * @return std::string response
 */
std::string Uploader::responseMessage() {
    return this->v_responseMessage;
}

size_t Uploader::responseCallback(void* data, size_t size, size_t n, std::string* response) {
    size_t totalSize = n * size;
    response->append(static_cast<char*>(data), totalSize);
    return totalSize;
}
