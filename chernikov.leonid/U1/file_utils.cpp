#include "file_utils.hpp"
#include <fstream>
#include <ostream>
#include <cstring>

int chernikov::parseArguments(int argc, char *argv[], FileConfig &config)
{
  config.inputFile = "";
  config.outputFile = "";
  config.useStdin = true;
  config.useStdout = true;
  config.sameFile = false;

  if (argc > 3)
  {
    return 2;
  }

  bool hasInput = false;
  bool hasOutput = false;

  for (int i = 1; i < argc; ++i)
  {
    if (std::strncmp(argv[i], "in:", 3) == 0)
    {
      if (hasInput)
      {
        return 1;
      }
      config.inputFile = argv[i] + 3;
      config.useStdin = false;
      hasInput = true;
    }
    else if (std::strncmp(argv[i], "out:", 4) == 0)
    {
      if (hasOutput)
      {
        return 1;
      }
      config.outputFile = argv[i] + 4;
      config.useStdout = false;
      hasOutput = true;
    }
    else
    {
      return 1;
    }
  }

  if (!config.useStdin && !config.useStdout && config.inputFile == config.outputFile)
  {
    config.sameFile = true;
  }

  return 0;
}

bool chernikov::openFiles(
    const FileConfig &config,
    std::ifstream &inputFileStream,
    std::ofstream &outputFileStream,
    std::istream *&input,
    std::ostream *&output)
{
  if (config.useStdin)
  {
    input = &std::cin;
  }
  else
  {
    inputFileStream.open(config.inputFile);
    if (!inputFileStream.is_open())
    {
      return false;
    }
    input = &inputFileStream;
  }

  if (config.useStdout)
  {
    output = &std::cout;
  }
  else if (config.sameFile)
  {
    output = &std::cout;
  }
  else
  {
    outputFileStream.open(config.outputFile);
    if (!outputFileStream.is_open())
    {
      if (!config.useStdin)
      {
        inputFileStream.close();
      }
      return false;
    }
    output = &outputFileStream;
  }

  return true;
}

void chernikov::closeFiles(std::ifstream &inputFileStream, std::ofstream &outputFileStream)
{
  if (inputFileStream.is_open())
  {
    inputFileStream.close();
  }
  if (outputFileStream.is_open())
  {
    outputFileStream.close();
  }
}
