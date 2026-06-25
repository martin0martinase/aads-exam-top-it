#ifndef CHERNIKOV_INPUT_PARSER_HPP
#define CHERNIKOV_INPUT_PARSER_HPP

#include <iosfwd>
#include <string>
#include "person.hpp"

namespace chernikov
{

  struct ParseResult
  {
    std::size_t validCount;
    std::size_t ignoredCount;
  };

  Person parseLine(const std::string &line, bool &success);

  ParseResult parseInput(std::istream &input, std::ostream &output);

  std::string parseInputToString(std::istream &input, ParseResult &result);

}

#endif
