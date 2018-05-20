// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "boost/algorithm/string/predicate.hpp"
#include "src/crow_all.h"

#include "src/template.hpp"

namespace backstage {
namespace renderer {
TemplateEngine::TemplateEngine(std::string templatePath)
    : templatePath(templatePath), templates() {
    LoadTemplates();
}

TemplateEngine *TemplateEngine::GetEngine(std::string tmplPath) {
    if (templateEngine == NULL) {
        templateEngine = new TemplateEngine(tmplPath);
    }

    return templateEngine;
}

bool TemplateEngine::HasTemplate(std::string tmpl) {
    CROW_LOG_INFO << "Looking for template " << tmpl << ":";
    CROW_LOG_INFO << "Available Templates:";
    for (auto it = templates.begin(); it != templates.end(); ++it) {
        CROW_LOG_INFO << it->first;
    }
    return templates.find(tmpl) != templates.end();
}

std::string TemplateEngine::Render(std::string tmpl, Grantlee::Context c) {
    if (!HasTemplate(tmpl)) {
        CROW_LOG_WARNING << "[Backstage-Renderer] Failed to locate template "
                         << tmpl << "!";
        // TODO(heynemann): Fail somehow!
        return "";
    }
    auto t = templates[tmpl];
    auto res = t->render(&c);

    std::string utf8_text = res.toUtf8().constData();
    return utf8_text;
}

void TemplateEngine::LoadTemplates() {
    CROW_LOG_INFO << "[Backstage-Renderer] Loading templates...";
    auto tmplPath = std::string(templatePath);
    if (tmplPath.empty()) {
        CROW_LOG_ERROR << "[Backstage-Renderer] Can't load templates without a "
                          "templatePath set.";
        // TODO(heynemann): Fail somehow!
    }

    CROW_LOG_DEBUG << "[Backstage-Renderer] Clearing templates...";
    templates.clear();

    auto engine = new Grantlee::Engine(0);
    auto loader = QSharedPointer<Grantlee::InMemoryTemplateLoader>::create();

    auto templateList = std::vector<std::string>{};

    fs::path templateDir(tmplPath);

    for (boost::filesystem::recursive_directory_iterator end, dir(tmplPath);
         dir != end; ++dir) {
        // auto folder = boost::filesystem::relative(dir, tmplPath);
        // std::cout << folder << "\n";                  // full path
        // std::cout << dir->path().filename() << "\n";  // just last bit
        auto is_html = dir->path().extension().string() == ".html";
        if (is_html) {
            auto tmplKey = relativeTo(templateDir, dir->path()).string();
            auto tmplFile = dir->path().string();
            CROW_LOG_INFO << "[Backstage-Renderer] Template " << tmplKey
                          << " found at '" << tmplFile << "'.";
            templateList.push_back(tmplKey);

            auto qKey = QString::fromUtf8(tmplKey.c_str());
            loader->setTemplate(qKey, readFile(tmplFile).c_str());
        }
    }

    CROW_LOG_INFO << "[Backstage-Renderer] All templates added to engine...";

    engine->addTemplateLoader(loader);

    for (auto i = 0; i < templateList.size(); i++) {
        auto qKey = QString::fromUtf8(templateList[i].c_str());
        CROW_LOG_INFO << "[Backstage-Renderer] Compiling template "
                      << templateList[i] << "...";
        auto tmpl = engine->loadByName(qKey);
        templates[templateList[i]] = tmpl;
    }
}
}  // namespace renderer
}  // namespace backstage
