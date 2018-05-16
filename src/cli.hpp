// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#pragma once

#include <string>

namespace backstage {
namespace renderer {

class CliRoot {
 public:
    bool help = false;
    std::string host = "localhost";
    int port = 8888;
    int workers = 8;
    int logLevel = 0;
    std::string configFile = "config.yaml";

    CliRoot() {}
    void ParseArgs(int argc, char *argv[]);
    void PrintUsage();
};
}  // namespace renderer
}  // namespace backstage
