// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#pragma once

#include <map>
#include <memory>
#include <string>

#include "grantlee/engine.h"
#include "grantlee/template.h"
#include "grantlee/templateloader.h"

namespace backstage {
namespace renderer {
class TemplateEngine {
 public:
    std::map<std::string, Grantlee::Template> templates;
    std::string templatePath;
    explicit TemplateEngine(std::string templatePath);

    void LoadTemplates();
};
}  // namespace renderer
}  // namespace backstage
