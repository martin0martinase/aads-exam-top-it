#ifndef CHERNIKOV_FILE_UTILS_HPP
#define CHERNIKOV_FILE_UTILS_HPP

#include <string>
#include <vector>

namespace chernikov
{

  struct ProgramConfig
  {
    std::string personsFile;
    std::vector<std::string> meetsFiles;
    bool hasPersonsFile;
  };

  bool parseArguments(int argc, char *argv[], ProgramConfig &config);

}

#endif
