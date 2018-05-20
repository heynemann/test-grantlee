// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "boost/filesystem.hpp"
#include "grantlee/engine.h"
#include "grantlee/template.h"
#include "grantlee/templateloader.h"

namespace fs = boost::filesystem;

namespace backstage {
namespace renderer {

class TemplateEngine {
 public:
    explicit TemplateEngine(std::string templatePath);
    static TemplateEngine *GetEngine(std::string templatePath);
    bool HasTemplate(std::string tmpl);
    std::string Render(std::string tmpl, Grantlee::Context c);
    std::map<std::string, Grantlee::Template> templates;

 private:
    std::string templatePath;

    void LoadTemplates();

    static fs::path relativeTo(fs::path from, fs::path to) {
        // Start at the root path and while they are the same then do
        // nothing then when they first diverge take the entire from path,
        // swap it with
        // '..' segments, and then append the remainder of the to path.
        fs::path::const_iterator fromIter = from.begin();
        fs::path::const_iterator toIter = to.begin();

        // Loop through both while they are the same to find nearest common
        // directory
        while (fromIter != from.end() && toIter != to.end() &&
               (*toIter) == (*fromIter)) {
            ++toIter;
            ++fromIter;
        }

        // Replace from path segments with '..' (from => nearest common
        // directory)
        fs::path finalPath;
        while (fromIter != from.end()) {
            finalPath /= "..";
            ++fromIter;
        }

        // Append the remainder of the to path (nearest common directory =>
        // to)
        while (toIter != to.end()) {
            finalPath /= *toIter;
            ++toIter;
        }

        return finalPath;
    }

    static std::string readFile(const std::string &fileName) {
        std::ifstream ifs(fileName.c_str(),
                          std::ios::in | std::ios::binary | std::ios::ate);

        std::ifstream::pos_type fileSize = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        std::vector<char> bytes(fileSize);
        ifs.read(bytes.data(), fileSize);

        return std::string(bytes.data(), fileSize);
    }
};

thread_local static TemplateEngine *templateEngine;
}  // namespace renderer
}  // namespace backstage
