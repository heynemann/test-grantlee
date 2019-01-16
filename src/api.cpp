// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

//#include <QJsonDocument>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <nlohmann/json.hpp>
#include <inja.hpp>

#include "src/api.hpp"

using json = nlohmann::json;
using namespace inja;

namespace backstage {
namespace renderer {
RendererApp::RendererApp(std::unique_ptr<CliRoot> client)
    : client(std::move(client)) {
    ConfigureLogLevel();
    ConfigureRoutes();
}

RendererApp::~RendererApp() {}

void RendererApp::ConfigureLogLevel() {
    switch (client->logLevel) {
        case 1:
            app.loglevel(crow::LogLevel::WARNING);
            break;
        case 2:
            app.loglevel(crow::LogLevel::INFO);
            break;
        case 3:
            app.loglevel(crow::LogLevel::DEBUG);
            break;
        default:
            app.loglevel(crow::LogLevel::ERROR);
            break;
    }
}

void RendererApp::ConfigureRoutes() {
    const auto working = "WORKING";

    CROW_ROUTE(app, "/healthcheck")
    ([this, &working]() { return working; });

    CROW_ROUTE(app, "/").methods("GET"_method)(
        [this](const crow::request &req) {
            json data;
            data["name"] = "world";

            auto result = render("Hello {{ name }}!", data); // "Hello world!"
            return result;
        });

    CROW_ROUTE(app, "/render")
        .methods("POST"_method)([this](const crow::request &req) {
            // Get Template Path
            auto templ = req.url_params.get("templ");
            CROW_LOG_INFO << "template == '" << templ << "'";
            if (!templ) {
                CROW_LOG_WARNING
                    << "[Backstage-Renderer] No template was passed in ?templ "
                       "querystring.";
                return crow::response(400);
            }

            auto data = json::parse(req.body.c_str());

            CROW_LOG_INFO << "Template Path: " << client->templatePath;
            Environment env {client->templatePath, "/tmp"};
            auto temp = env.parse_template(templ);

            if (temp.content == "") {
                CROW_LOG_WARNING
                    << "[Backstage-Renderer] Template at path " << templ <<
                       " could not be found.";
                return crow::response(400);
            }


            CROW_LOG_INFO << "Template contents: " << temp.content;
            std::string result = env.render(temp, data);
            CROW_LOG_WARNING << "Result: " << result;

            crow::response resp;
            resp.code = 200;
            resp.add_header("Content-Type", "text/plain");
            resp.write(result);
            return resp;
        });
}

void RendererApp::PrintRunningOptions() {
    std::cout << "Backstage Renderer\n";
    std::cout << "------------------\n\n";
    std::cout << "Running with Options:\n";
    std::cout << "  Log Level: " << client->logLevel << "\n";
    std::cout << "  Workers: " << client->workers << "\n";
    std::cout << "  Config File: " << client->configFile << "\n";
    std::cout << "  Templates Path: " << client->templatePath << "\n";
    std::cout << "  URL: http://" << client->host << ":" << client->port
              << "\n";
}

void RendererApp::Run() {
    PrintRunningOptions();
    app.bindaddr(client->host)
        .port(client->port)
        .concurrency(client->workers)
        .run();
}
}  // namespace renderer
}  // namespace backstage
