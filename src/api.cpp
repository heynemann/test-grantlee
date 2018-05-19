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
    InitializeTemplateEngine();
    ConfigureRoutes();
}

RendererApp::~RendererApp() { delete engine; }

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

void RendererApp::InitializeTemplateEngine() {
    engine = new TemplateEngine(client->templatePath);
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

            auto t = engine->templates["templ"];
            auto res = t->render(&c);

            std::string utf8_text = res.toUtf8().constData();
            return utf8_text;
        });

    // CROW_ROUTE(app, "/render")
    //.methods("POST"_method)([&templates](const crow::request &req) {
    //// Get Template Path
    // auto templ = req.url_params.get("templ");

    //// Template not found!
    // if (templates.count(templ) == 0) return crow::response(400);

    //// Get Context
    // auto doc = QJsonDocument::fromJson(req.body.c_str());

    //// Context could not be parsed!
    // if (doc.isNull()) return crow::response(400);

    // QVariantHash mapping = qvariant_cast<QVariantHash>(doc.toVariant());
    // Grantlee::Context c(mapping);

    // auto t = templates[templ];
    // auto res = t->render(&c);
    // std::string utf8_text = res.toUtf8().constData();
    // return crow::response(utf8_text);
    //});
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
