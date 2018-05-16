#define MAINCLIFILE

#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "boost/filesystem.hpp"

#include "src/argh.h"
#include "src/cli.hpp"

namespace backstage {
namespace renderer {

void parse_args(int argc, char *argv[]) {
  argh::parser cmdl({"-h", "--help", "-v", "-vv", "-vvv", "-b", "--bind", "-p",
                     "--port", "-c", "--config", "-w", "--workers"});
  cmdl.parse(argc, argv);

  parsed_options.help = false;
  if (cmdl[{"-h", "--help"}]) {
    parsed_options.help = true;
  }

  int log_level = 0;
  if (cmdl[{"-v"}]) {
    log_level = 1;
  }
  if (cmdl[{"-vv"}]) {
    log_level = 2;
  }
  if (cmdl[{"-vvv"}]) {
    log_level = 3;
  }
  parsed_options.log_level = log_level;

  int port;
  cmdl({"-p", "--port"}, 8888) >> port;
  parsed_options.port = port;

  parsed_options.host = cmdl({"-b", "--bind"}, "0.0.0.0").str();

  int workers;
  cmdl({"-w", "--workers"}, 8) >> workers;
  parsed_options.workers = workers;

  auto const cfg = cmdl({"-c", "--config"}, "config.yml").str();
  auto const abspath = boost::filesystem::absolute(cfg);
  parsed_options.config_file = abspath.string();
  if (!boost::filesystem::exists(parsed_options.config_file)) {
    std::cout << "No configuration file could be found at " << abspath.string()
              << ". Terminating...\n";
    exit(1);
  }
}

void print_usage() {
  std::cout
      << "Backstage Renderer v0.1.0\n"
      << "-------------------------\n"
      << "Use this command line app to run Backstage Renderer.\n\n"
      << "The following flags are available:\n"
      << "-v, -vv, -vvv             # Log Level: Default - ERROR, -v WARN,\n"
      << "                          # -vv INFO, -vvv ERROR\n"
      << "-c[--config=] config.yml  # Path to config file. MUST EXIST!\n"
      << "-b[--bind=] 0.0.0.0       # IP Address to Bind HTTP Server to\n"
      << "-p[--port=] 8888          # Port to Bind HTTP Server to\n"
      << "-w[--workers=] 8          # Number of Worker Threads in the API\n";
}
}
}
