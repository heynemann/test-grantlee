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
#include "src/template.hpp"

namespace backstage {
namespace renderer {

class RendererApp {
 private:
    crow::SimpleApp app;
    std::unique_ptr<CliRoot> client;
    TemplateEngine *engine;

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
