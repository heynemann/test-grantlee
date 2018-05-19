// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "boost/filesystem.hpp"

#include "src/argh.h"
#include "src/cli.hpp"

namespace backstage {
namespace renderer {

void CliRoot::ParseArgs(int argc, char *argv[]) {
    argh::parser cmdl({"-h", "--help", "-v", "-vv", "-vvv", "-b", "--bind",
                       "-p", "--port", "-c", "--config", "-w", "--workers",
                       "--version", "-t", "--templates"});
    cmdl.parse(argc, argv);

    help = false;
    if (cmdl[{"-h", "--help"}]) {
        help = true;
    }

    version = false;
    if (cmdl[{"--version"}]) {
        version = true;
    }

    int ll = 0;
    if (cmdl[{"-v"}]) {
        ll = 1;
    }
    if (cmdl[{"-vv"}]) {
        ll = 2;
    }
    if (cmdl[{"-vvv"}]) {
        ll = 3;
    }
    logLevel = ll;

    int p;
    cmdl({"-p", "--port"}, 8888) >> p;
    port = p;

    host = cmdl({"-b", "--bind"}, "0.0.0.0").str();

    int w;
    cmdl({"-w", "--workers"}, 8) >> w;
    workers = w;

    auto const tmpl = cmdl({"-t", "--templates"}, "templates").str();
    auto const tmplAbs = boost::filesystem::absolute(tmpl);
    templatePath = tmplAbs.string();
    if (!boost::filesystem::exists(templatePath)) {
        std::cout << "The templates path specified could not be found at "
                  << tmplAbs.string() << ". Terminating...\n";
        exit(1);
    }

    auto const cfg = cmdl({"-c", "--config"}, "config.yml").str();
    auto const cfgAbs = boost::filesystem::absolute(cfg);
    configFile = cfgAbs.string();
    if (!boost::filesystem::exists(configFile)) {
        std::cout << "No configuration file could not be found at "
                  << cfgAbs.string() << ". Terminating...\n";
        exit(1);
    }
}

void CliRoot::PrintUsage(int major, int minor, int revision) {
    std::cout << "Backstage Renderer v" << major << "." << minor << "."
              << revision << "\n"
              << "-------------------------\n"
              << "Use this command line app to run Backstage Renderer.\n\n"
              << "The following flags are available:\n"
              << "-h                        # Show this help message\n"
              << "--version                 # Show Backstage-Rendeer version\n"
              << "-v, -vv, -vvv             # Log Level:\n"
              << "                          # * Default: Error\n"
              << "                          # * -v:      Warning\n"
              << "                          # * -vv:     Info\n"
              << "                          # * -vvv:    Debug\n"
              << "-c[--config=] config.yml  # Path to config file.\n"
              << "                          # MUST EXIST!\n"
              << "-b[--bind=] 0.0.0.0       # IP Address to Bind Server\n"
              << "-p[--port=] 8888          # Port to Bind HTTP Server to\n"
              << "-w[--workers=] 8          # Number of Worker Threads\n";
}

void CliRoot::PrintVersion(int major, int minor, int revision) {
    std::cout << "Backstage Renderer v" << major << "." << minor << "."
              << revision << "\n";
}
}  // namespace renderer
}  // namespace backstage
