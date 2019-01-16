// Copyright 2018 Bernardo Heynemann <heynemann@gmail.com>
// Backstage Renderer

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "boost/filesystem.hpp"

#include "src/argh.h"
#include "src/crow_all.h"

#include "src/api.hpp"
#include "src/cli.hpp"

#include "src/api_config.h"

using namespace backstage::renderer;

int main(int argc, char *argv[]) {
    std::unique_ptr<CliRoot> cli(new CliRoot);
    cli->ParseArgs(argc, argv);

    if (cli->help) {
        cli->PrintUsage(API_VERSION_MAJOR, API_VERSION_MINOR,
                        API_VERSION_REVISION);
        return 0;
    }

    if (cli->version) {
        cli->PrintVersion(API_VERSION_MAJOR, API_VERSION_MINOR,
                          API_VERSION_REVISION);
        return 0;
    }

    RendererApp app(std::move(cli));
    app.Run();

    return 0;
}
