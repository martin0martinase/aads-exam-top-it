#include <iostream>
#include <fstream>
#include <sstream>
#include "file_utils.hpp"
#include "input_parser.hpp"

int main(int argc, char *argv[])
{
  chernikov::FileConfig config;
  int parseResult = chernikov::parseArguments(argc, argv, config);
  if (parseResult == 1)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }
  if (parseResult == 2)
  {
    std::cerr << "Invalid arguments\n";
    return 0;
  }

  std::ifstream inputFileStream;
  std::ofstream outputFileStream;
  std::istream *input = nullptr;
  std::ostream *output = nullptr;

  if (!chernikov::openFiles(config, inputFileStream, outputFileStream, input, output))
  {
    std::cerr << "Failed to open files\n";
    return 2;
  }

  chernikov::ParseResult result;

  if (config.sameFile)
  {
    std::string outputStr = chernikov::parseInputToString(*input, result);
    chernikov::closeFiles(inputFileStream, outputFileStream);
    outputFileStream.open(config.outputFile);
    if (!outputFileStream.is_open())
    {
      std::cerr << "Failed to open output file\n";
      return 2;
    }
    outputFileStream << outputStr;
    outputFileStream.close();
  }
  else
  {
    result = chernikov::parseInput(*input, *output);
    chernikov::closeFiles(inputFileStream, outputFileStream);
  }

  std::cerr << result.validCount << " " << result.ignoredCount << "\n";

  return 0;
}
