#include "file_utils.hpp"
#include <iostream>
#include <cstring>

bool chernikov::parseArguments(int argc, char *argv[], ProgramConfig &config)
{
  config.personsFile = "";
  config.meetsFiles.clear();
  config.hasPersonsFile = false;

  if (argc < 2)
  {
    std::cerr << "Invalid number of arguments\n";
    return false;
  }

  for (int i = 1; i < argc; ++i)
  {
    if (std::strncmp(argv[i], "in:", 3) == 0)
    {
      if (config.hasPersonsFile)
      {
        std::cerr << "Duplicate input file argument\n";
        return false;
      }
      config.personsFile = argv[i] + 3;
      config.hasPersonsFile = true;
    }
    else if (std::strncmp(argv[i], "data:", 5) == 0)
    {
      config.meetsFiles.push_back(argv[i] + 5);
    }
    else
    {
      std::cerr << "Invalid argument: " << argv[i] << "\n";
      return false;
    }
  }

  if (config.meetsFiles.empty())
  {
    std::cerr << "Missing required data file argument\n";
    return false;
  }

  return true;
}
