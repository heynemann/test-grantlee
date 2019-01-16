// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#pragma once

#include <memory>
#include <string>

#include "src/cli.hpp"
#include "src/crow_all.h"

namespace backstage {
namespace renderer {

class RendererApp {
 private:
    crow::SimpleApp app;
    std::unique_ptr<CliRoot> client;

 public:
    explicit RendererApp(std::unique_ptr<CliRoot> client);
    // destructor declaration
    virtual ~RendererApp();

    void ConfigureLogLevel();
    void InitializeTemplateEngine();
    void ConfigureRoutes();
    void Run();
    void PrintRunningOptions();
};
}  // namespace renderer
}  // namespace backstage
