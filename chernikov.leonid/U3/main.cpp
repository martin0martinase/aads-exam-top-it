#include <iostream>
#include <fstream>
#include <string>
#include "commands.hpp"
#include "file_utils.hpp"

int main(int argc, char *argv[])
{
  chernikov::ProgramConfig config;
  if (!chernikov::parseArguments(argc, argv, config))
  {
    return 1;
  }

  for (const auto &meetsFile : config.meetsFiles)
  {
    std::ifstream file(meetsFile);
    if (!file.is_open())
    {
      std::cerr << "Failed to open meets file: " << meetsFile << "\n";
      return 2;
    }
    file.close();
  }

  if (config.hasPersonsFile)
  {
    std::ifstream personsFile(config.personsFile);
    if (!personsFile.is_open())
    {
      std::cerr << "Failed to open persons file\n";
      return 2;
    }
    personsFile.close();
  }

  chernikov::AppState state;
  chernikov::initAppState(state);

  if (config.hasPersonsFile)
  {
    chernikov::loadPersonsFromFile(state, config.personsFile);
  }

  for (const auto &meetsFile : config.meetsFiles)
  {
    chernikov::loadMeetsFromFile(state, meetsFile);
  }

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    chernikov::executeCommand(state, line);
  }

  chernikov::destroyAppState(state);

  return 0;
}
