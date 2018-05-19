// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#pragma once

#include <string>

namespace backstage {
namespace renderer {

class CliRoot {
 public:
    bool help = false;
    bool version = false;
    std::string host = "localhost";
    int port = 8888;
    int workers = 8;
    int logLevel = 0;
    std::string configFile = "config.yaml";
    std::string templatePath = "./templates";

    CliRoot() {}
    void ParseArgs(int argc, char *argv[]);
    void PrintUsage(int major, int minor, int revision);
    void PrintVersion(int major, int minor, int revision);
};
}  // namespace renderer
}  // namespace backstage
