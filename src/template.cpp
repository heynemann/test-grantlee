// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "src/template.hpp"

namespace backstage {
namespace renderer {
TemplateEngine::TemplateEngine(std::string templatePath)
    : templatePath(templatePath), templates() {
    LoadTemplates();
}

void TemplateEngine::LoadTemplates() {
    std::cout << "Loading templates...\n";
    auto engine = new Grantlee::Engine(0);
    auto loader = QSharedPointer<Grantlee::InMemoryTemplateLoader>::create();
    loader->setTemplate("templ", "<html><body>Hello {{name}}!</body></html>");
    engine->addTemplateLoader(loader);

    auto t = engine->loadByName("templ");
    templates["templ"] = t;
}
}  // namespace renderer
}  // namespace backstage
