// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <QJsonDocument>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "boost/filesystem.hpp"
#include "grantlee/engine.h"
#include "grantlee/template.h"

#include "src/argh.h"
#include "src/cli.hpp"
#include "src/crow_all.h"

// #include "src/apiConfig.h"

using namespace backstage::renderer;

int main(int argc, char *argv[]) {
    CliRoot cli;
    cli.ParseArgs(argc, argv);

    if (cli.help) {
        cli.PrintUsage();
        return 0;
    }

    std::cout << "Backstage Renderer\n";
    std::cout << "------------------\n\n";
    std::cout << "Running with Options:\n";
    std::cout << "  Log Level: " << cli.logLevel << "\n";
    std::cout << "  Workers: " << cli.workers << "\n";
    std::cout << "  Config File: " << cli.configFile << "\n";
    std::cout << "  Address: http://" << cli.host << ":" << cli.port << "\n";

    auto temp = R"(
    <html>
    <head>
    <title>{% block title %}My Stuff{% endblock %}</title>
    </head>
    <body>
    <div class="sidebar">
    {% block sidebar %}
    {% endblock sidebar %}
    </div>
    <div class="main_content">
    {% block content %}
     Hello {{ name }}!
    {% endblock content %}
    </div>
    </body>
    </html>
    )";
    auto temp2 = R"(
    <html>
    <head>
    <title>{% block title %}My Stuff{% endblock %}</title>
    </head>
    <body>
    <div class="sidebar">
    {% block sidebar %}
    {% endblock sidebar %}
    </div>
    <div class="main_content">
    {% block content %}
    <ul>
    <li>Key1: {{ key1 }}</li>
    <li>Key2: {{ key2 }}</li>
    </ul>
    {% endblock content %}
    </div>
    </body>
    </html>
    )";

    auto *engine = new Grantlee::Engine(0);
    auto t = engine->newTemplate(temp, "my_template_name");
    auto t2 = engine->newTemplate(temp2, "my_template_name_2");

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([&t]() {
        QVariantMap hash;
        hash.insert("name", "Grainne");
        QVariantHash mapping = qvariant_cast<QVariantHash>(hash);
        Grantlee::Context c(mapping);

        auto res = t->render(&c);

        std::string utf8_text = res.toUtf8().constData();
        return utf8_text;
    });

    CROW_ROUTE(app, "/add_json")
        .methods("POST"_method)([&t2](const crow::request &req) {
            auto body = QString::fromUtf8(req.body.c_str());
            auto doc = QJsonDocument::fromJson(body.toUtf8());
            if (doc.isNull()) return crow::response(400);

            QVariantHash mapping = qvariant_cast<QVariantHash>(doc.toVariant());
            Grantlee::Context c(mapping);

            auto res = t2->render(&c);
            std::string utf8_text = res.toUtf8().constData();
            return crow::response(utf8_text);
        });

    switch (cli.logLevel) {
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

    app.bindaddr(cli.host).port(cli.port).concurrency(cli.workers).run();
    return 0;
}
