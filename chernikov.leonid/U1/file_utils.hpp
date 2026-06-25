#ifndef CHERNIKOV_FILE_UTILS_HPP
#define CHERNIKOV_FILE_UTILS_HPP

#include <string>
#include <iosfwd>
#include <iostream>

namespace chernikov
{

  struct FileConfig
  {
    std::string inputFile;
    std::string outputFile;
    bool useStdin;
    bool useStdout;
    bool sameFile;
  };

  int parseArguments(int argc, char *argv[], FileConfig &config);

  bool openFiles(
      const FileConfig &config,
      std::ifstream &inputFileStream,
      std::ofstream &outputFileStream,
      std::istream *&input,
      std::ostream *&output);

  void closeFiles(std::ifstream &inputFileStream, std::ofstream &outputFileStream);

}

#endif
