// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#pragma once

#include <memory>
#include <string>

#include "grantlee/engine.h"
#include "grantlee/template.h"
#include "grantlee/templateloader.h"

#include "src/cli.hpp"
#include "src/crow_all.h"

namespace backstage {
namespace renderer {
auto const temp = R"(
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
auto const temp2 = R"(
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

auto const temp3 = R"(
  {% extends "my_template_name_2" %}
  {% block sidebar %}
    Testing inheritance!
  {% endblock sidebar %}
)";

class RendererApp {
 private:
    crow::SimpleApp app;
    Grantlee::Engine *templateEngine;

 public:
    std::unique_ptr<CliRoot> client;

    explicit RendererApp(std::unique_ptr<CliRoot> client);

    void ConfigureLogLevel();
    void InitializeTemplateEngine();
    void ConfigureRoutes();
    void Run();
    void PrintRunningOptions();
};
}  // namespace renderer
}  // namespace backstage
