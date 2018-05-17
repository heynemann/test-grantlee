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
    // InitializeTemplateEngine();
    // ConfigureRoutes();
}

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
    std::cout << "Configuring templates...\n";

    templateEngine = new Grantlee::Engine(0);
    auto loader = QSharedPointer<Grantlee::InMemoryTemplateLoader>::create();
    loader->setTemplate("my_template_name", temp);
    loader->setTemplate("my_template_name_2", temp2);
    templateEngine->addTemplateLoader(loader);
}

void RendererApp::ConfigureRoutes() {
    auto t = templateEngine->loadByName("my_template_name");

    CROW_ROUTE(app, "/")
    ([this, &t]() {
        QVariantMap hash;
        hash.insert("name", "Grainne");
        QVariantHash mapping = qvariant_cast<QVariantHash>(hash);
        Grantlee::Context c(mapping);

        // auto t = templateEngine->loadByName("my_template_name");
        auto res = t->render(&c);

        std::string utf8_text = res.toUtf8().constData();
        return utf8_text;
    });

    CROW_ROUTE(app, "/add_json")
        .methods("POST"_method)([this](const crow::request &req) {
            auto doc = QJsonDocument::fromJson(req.body.c_str());
            if (doc.isNull()) return crow::response(400);

            QVariantHash mapping = qvariant_cast<QVariantHash>(doc.toVariant());
            Grantlee::Context c(mapping);

            auto t = templateEngine->loadByName("my_template_name_2");
            auto res = t->render(&c);
            std::string utf8_text = res.toUtf8().constData();
            return crow::response(utf8_text);
        });
}

void RendererApp::PrintRunningOptions() {
    std::cout << "Backstage Renderer\n";
    std::cout << "------------------\n\n";
    std::cout << "Running with Options:\n";
    std::cout << "  Log Level: " << client->logLevel << "\n";
    std::cout << "  Workers: " << client->workers << "\n";
    std::cout << "  Config File: " << client->configFile << "\n";
    std::cout << "  URL: http://" << client->host << ":" << client->port
              << "\n";
}
void RendererApp::Run() {
    auto *engine = new Grantlee::Engine(0);
    auto templates = std::map<std::string, Grantlee::Template>{
        {"my_template_name", engine->newTemplate(temp, "my_template_name")},
        {"my_template_name_2", engine->newTemplate(temp2, "my_template_name")},
        {"my_template_name_3", engine->newTemplate(temp3, "my_template_name")},
    };

    CROW_ROUTE(app, "/")
    ([&templates]() {
        QVariantMap hash;
        hash.insert("name", "Grainne");
        QVariantHash mapping = qvariant_cast<QVariantHash>(hash);
        Grantlee::Context c(mapping);

        auto t = templates["my_template_name"];
        auto res = t->render(&c);

        std::string utf8_text = res.toUtf8().constData();
        return utf8_text;
    });

    // TODO(heynemann): This won't work without a proper template loader
    CROW_ROUTE(app, "/inherit")
    ([&templates]() {
        QVariantMap hash;
        hash.insert("name", "Grainne");
        QVariantHash mapping = qvariant_cast<QVariantHash>(hash);
        Grantlee::Context c(mapping);

        auto t = templates["my_template_name_3"];
        auto res = t->render(&c);

        std::string utf8_text = res.toUtf8().constData();
        return utf8_text;
    });

    CROW_ROUTE(app, "/add_json")
        .methods("POST"_method)([&templates](const crow::request &req) {
            auto doc = QJsonDocument::fromJson(req.body.c_str());
            if (doc.isNull()) return crow::response(400);

            QVariantHash mapping = qvariant_cast<QVariantHash>(doc.toVariant());
            Grantlee::Context c(mapping);

            auto t = templates["my_template_name_2"];
            auto res = t->render(&c);
            std::string utf8_text = res.toUtf8().constData();
            return crow::response(utf8_text);
        });

    PrintRunningOptions();
    app.bindaddr(client->host)
        .port(client->port)
        .concurrency(client->workers)
        .run();

    free(engine);
}
}  // namespace renderer
}  // namespace backstage
