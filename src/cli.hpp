#pragma once

#ifdef MAINCLIFILE
#define EXTERN
#else
#define EXTERN extern
#endif

#include <string>

namespace backstage {
namespace renderer {

EXTERN struct CommandArgs {
  bool help;
  std::string host;
  int port;
  int workers;
  int log_level;
  std::string config_file;
} parsed_options;

void parse_args(int argc, char *argv[]);

void print_usage();

} // namespace renderer
} // namespace backstage
