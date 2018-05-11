// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// A simple program that computes the square root of a number

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <QJsonDocument>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "crow_all.h"
#include "grantlee/engine.h"
#include "grantlee/template.h"

// #include "src/apiConfig.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    // fprintf(stdout, "%s Version %d.%d.%d\n", argv[0], API_VERSION_MAJOR,
    // API_VERSION_MINOR, API_VERSION_REVISION);
    fprintf(stdout, "Usage: %s number_of_repetitions\n", argv[0]);
    return 1;
  }
  auto repetitions = atof(argv[1]);
  // auto outputValue = sqrt(inputValue);
  // fprintf(stdout, "The square root of %g is %g\n", inputValue, outputValue);

  // const std::map<std::string, std::string> capitals{{"Poland", "Warsaw"},
  //{"USA", "Washington"},
  //{"France", "Paris"},
  //{"UK", "London"},
  //{"Germany", "Berlin"}};

  //// can you write code that shows prints Country: Capital ?
  //// eg "Poland : Warsaw" ?
  // for (const auto & [ k, v ] : capitals) {
  // std::cout << k << " : " << v << "\n";
  //}
  //
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

  // std::cout << "\n\nBuilding " << repetitions << " templates...\n";
  // for (auto i = 0; i < repetitions; i++)
  // t->render(&c); // Returns "My name is Grainne."
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
        if (doc.isNull())
          return crow::response(400);

        QVariantHash mapping = qvariant_cast<QVariantHash>(doc.toVariant());
        Grantlee::Context c(mapping);

        auto res = t2->render(&c);
        std::string utf8_text = res.toUtf8().constData();
        return crow::response(utf8_text);
      });

  app.loglevel(crow::LogLevel::ERROR);

  app.port(18080).concurrency(repetitions).run();
  // app.port(18080).run();
  return 0;
}
