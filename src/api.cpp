// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#include <QJsonDocument>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "src/api.hpp"

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
            std::string name = "World";
            if (req.url_params.get("name") != NULL) {
                name = std::string(req.url_params.get("name"));
            }
            CROW_LOG_INFO << "name == '" << name << "'";

            QVariantMap hash;
            hash.insert("name", name.c_str());
            QVariantHash mapping = qvariant_cast<QVariantHash>(hash);
            Grantlee::Context c(mapping);

            auto eng = TemplateEngine::GetEngine(client->templatePath);
            return eng->Render("alone.html", c);
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

            auto eng = TemplateEngine::GetEngine(client->templatePath);

            // Template not found!
            if (!eng->HasTemplate(templ)) return crow::response(400);

            // Get Context
            auto doc = QJsonDocument::fromJson(req.body.c_str());

            // Context could not be parsed!
            if (doc.isNull()) return crow::response(400);

            QVariantHash mapping = qvariant_cast<QVariantHash>(doc.toVariant());
            Grantlee::Context c(mapping);

            // std::string utf8_text = res.toUtf8().constData();
            return crow::response(eng->Render(templ, c).c_str());
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
