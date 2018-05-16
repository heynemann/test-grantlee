// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// A simple program that computes the square root of a number

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <QJsonDocument>
#include <experimental/filesystem>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "boost/filesystem.hpp"
#include "grantlee/engine.h"
#include "grantlee/template.h"

#include "src/argh.h"
#include "src/crow_all.h"

// #include "src/apiConfig.h"

struct CommandArgs {
  bool help;
  std::string host;
  int port;
  int workers;
  int log_level;
  std::string config_file;
} parsed;

void parse_args(int argc, char *argv[]) {
  argh::parser cmdl(
      {"-h", "--help", "-v", "-vv", "-vvv", "-b", "--bind", "-p", "--port"});
  cmdl.parse(argc, argv);

  parsed.help = false;
  if (cmdl[{"-h", "--help"}]) {
    parsed.help = true;
  }

  int log_level = 0;
  if (cmdl[{"-v"}]) {
    log_level = 1;
  }
  if (cmdl[{"-vv"}]) {
    log_level = 2;
  }
  if (cmdl[{"-vvv"}]) {
    log_level = 3;
  }
  parsed.log_level = log_level;

  int port;
  cmdl({"-p", "--port"}, 8888) >> port;
  parsed.port = port;

  parsed.host = cmdl({"-b", "--bind"}, "0.0.0.0").str();

  int workers;
  cmdl({"-w", "--workers"}, 8) >> workers;
  parsed.workers = workers;

  auto const cfg = cmdl({"-c", "--config"}, "config.yml").str();
  auto const abspath = boost::filesystem::absolute(cfg);
  parsed.config_file = abspath.string();
  if (!boost::filesystem::exists(parsed.config_file)) {
    std::cout << "No configuration file could be found at " << abspath.string()
              << ". Terminating...\n";
    exit(1);
  }
}

void print_usage() {
  std::cout
      << "Backstage Renderer v0.1.0\n"
      << "-------------------------\n"
      << "Use this command line app to run Backstage Renderer.\n\n"
      << "The following flags are available:\n"
      << "-v, -vv, -vvv             # Log Level: Default - ERROR, -v WARN,"
      << "                          # -vv INFO, -vvv ERROR\n"
      << "-c[--config=] config.yml  # Path to config file. MUST EXIST!\n"
      << "-b[--bind=] 0.0.0.0       # IP Address to Bind HTTP Server to\n"
      << "-p[--port=] 8888          # Port to Bind HTTP Server to\n"
      << "-w[--workers=] 8          # Number of Worker Threads in the API\n";
}

int main(int argc, char *argv[]) {
  parse_args(argc, argv);

  if (parsed.help) {
    print_usage();
    return 0;
  }

  std::cout << "Log Level: " << parsed.log_level << "\n";
  std::cout << "Host and Port: " << parsed.host << ":" << parsed.port << "\n";
  std::cout << "Config File: " << parsed.config_file << "\n";
  // if (argc < 2) {
  //// fprintf(stdout, "%s Version %d.%d.%d\n", argv[0], API_VERSION_MAJOR,
  //// API_VERSION_MINOR, API_VERSION_REVISION);
  // fprintf(stdout, "Usage: %s number_of_repetitions\n", argv[0]);
  // return 1;
  //}
  // auto repetitions = atof(argv[1]);
  //// auto outputValue = sqrt(inputValue);
  //// fprintf(stdout, "The square root of %g is %g\n", inputValue,
  /// outputValue);

  //// const std::map<std::string, std::string> capitals{{"Poland", "Warsaw"},
  ////{"USA", "Washington"},
  ////{"France", "Paris"},
  ////{"UK", "London"},
  ////{"Germany", "Berlin"}};

  ////// can you write code that shows prints Country: Capital ?
  ////// eg "Poland : Warsaw" ?
  //// for (const auto & [ k, v ] : capitals) {
  //// std::cout << k << " : " << v << "\n";
  ////}
  ////
  // auto temp = R"(
  //<html>
  //<head>
  //<title>{% block title %}My Stuff{% endblock %}</title>
  //</head>
  //<body>
  //<div class="sidebar">
  //{% block sidebar %}
  //{% endblock sidebar %}
  //</div>
  //<div class="main_content">
  //{% block content %}
  // Hello {{ name }}!
  //{% endblock content %}
  //</div>
  //</body>
  //</html>
  //)";
  // auto temp2 = R"(
  //<html>
  //<head>
  //<title>{% block title %}My Stuff{% endblock %}</title>
  //</head>
  //<body>
  //<div class="sidebar">
  //{% block sidebar %}
  //{% endblock sidebar %}
  //</div>
  //<div class="main_content">
  //{% block content %}
  //<ul>
  //<li>Key1: {{ key1 }}</li>
  //<li>Key2: {{ key2 }}</li>
  //</ul>
  //{% endblock content %}
  //</div>
  //</body>
  //</html>
  //)";

  // auto *engine = new Grantlee::Engine(0);
  // auto t = engine->newTemplate(temp, "my_template_name");
  // auto t2 = engine->newTemplate(temp2, "my_template_name_2");

  //// std::cout << "\n\nBuilding " << repetitions << " templates...\n";
  //// for (auto i = 0; i < repetitions; i++)
  //// t->render(&c); // Returns "My name is Grainne."
  // crow::SimpleApp app;

  // CROW_ROUTE(app, "/")
  //([&t]() {
  // QVariantMap hash;
  // hash.insert("name", "Grainne");
  // QVariantHash mapping = qvariant_cast<QVariantHash>(hash);
  // Grantlee::Context c(mapping);

  // auto res = t->render(&c);

  // std::string utf8_text = res.toUtf8().constData();
  // return utf8_text;
  //});

  // CROW_ROUTE(app, "/add_json")
  //.methods("POST"_method)([&t2](const crow::request &req) {
  // auto body = QString::fromUtf8(req.body.c_str());
  // auto doc = QJsonDocument::fromJson(body.toUtf8());
  // if (doc.isNull())
  // return crow::response(400);

  // QVariantHash mapping = qvariant_cast<QVariantHash>(doc.toVariant());
  // Grantlee::Context c(mapping);

  // auto res = t2->render(&c);
  // std::string utf8_text = res.toUtf8().constData();
  // return crow::response(utf8_text);
  //});

  // app.loglevel(crow::LogLevel::ERROR);

  // app.port(18080).concurrency(repetitions).run();
  //// app.port(18080).run();
  return 0;
}
