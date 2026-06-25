#include "file_utils.hpp"
#include <iostream>
#include <cstring>

bool chernikov::parseArguments(int argc, char *argv[], ProgramConfig &config)
{
  config.personsFile = "";
  config.meetsFile = "";
  config.hasPersonsFile = false;

  if (argc < 2 || argc > 3)
  {
    std::cerr << "Invalid number of arguments\n";
    return false;
  }

  bool hasMeets = false;

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
      if (hasMeets)
      {
        std::cerr << "Duplicate data file argument\n";
        return false;
      }
      config.meetsFile = argv[i] + 5;
      hasMeets = true;
    }
    else
    {
      std::cerr << "Invalid argument: " << argv[i] << "\n";
      return false;
    }
  }

  if (!hasMeets)
  {
    std::cerr << "Missing required data file argument\n";
    return false;
  }

  return true;
}
