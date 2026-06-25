#ifndef CHERNIKOV_FILE_UTILS_HPP
#define CHERNIKOV_FILE_UTILS_HPP

#include <string>

namespace chernikov
{

  struct ProgramConfig
  {
    std::string personsFile;
    std::string meetsFile;
    bool hasPersonsFile;
  };

  bool parseArguments(int argc, char *argv[], ProgramConfig &config);

}

#endif
