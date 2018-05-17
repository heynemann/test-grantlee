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
                       "-p", "--port", "-c", "--config", "-w", "--workers"});
    cmdl.parse(argc, argv);

    help = false;
    if (cmdl[{"-h", "--help"}]) {
        help = true;
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

    auto const cfg = cmdl({"-c", "--config"}, "config.yml").str();
    auto const abspath = boost::filesystem::absolute(cfg);
    configFile = abspath.string();
    if (!boost::filesystem::exists(configFile)) {
        std::cout << "No configuration file could be found at "
                  << abspath.string() << ". Terminating...\n";
        exit(1);
    }
}

void CliRoot::PrintUsage() {
    std::cout << "Backstage Renderer v0.1.0\n"
              << "-------------------------\n"
              << "Use this command line app to run Backstage Renderer.\n\n"
              << "The following flags are available:\n"
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
}  // namespace renderer
}  // namespace backstage
