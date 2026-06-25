#include "input_parser.hpp"
#include "hash_table.hpp"
#include <istream>
#include <ostream>
#include <sstream>
#include <cctype>
#include <string>
#include <functional>

namespace
{

  bool isSpace(char c)
  {
    return c == ' ' || c == '\t';
  }

}

chernikov::Person chernikov::parseLine(const std::string &line, bool &success)
{
  success = false;
  Person person{0, ""};

  if (line.empty())
  {
    return person;
  }

  std::size_t pos = 0;
  std::size_t len = line.size();

  while (pos < len && isSpace(line[pos]))
  {
    ++pos;
  }

  if (pos >= len || !std::isdigit(line[pos]))
  {
    return person;
  }

  std::size_t id = 0;
  while (pos < len && std::isdigit(line[pos]))
  {
    id = id * 10 + (line[pos] - '0');
    ++pos;
  }

  person.id = id;

  while (pos < len && isSpace(line[pos]))
  {
    ++pos;
  }

  if (pos < len)
  {
    person.info = line.substr(pos);
  }

  if (person.info.empty())
  {
    return person;
  }

  success = true;
  return person;
}

chernikov::ParseResult chernikov::parseInput(std::istream &input, std::ostream &output)
{
  ParseResult result{0, 0};
  chernikov::HashTable<std::size_t, bool> seenIds;
  initHashTable(seenIds);

  std::string line;
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }

    bool success = false;
    Person person = parseLine(line, success);

    if (!success)
    {
      ++result.ignoredCount;
      continue;
    }

    if (hasInHashTable(seenIds, person.id))
    {
      ++result.ignoredCount;
      continue;
    }

    addToHashTable(seenIds, person.id, true);
    printPerson(person, output);
    ++result.validCount;
  }

  destroyHashTable(seenIds);

  if (result.validCount == 0 && result.ignoredCount == 0)
  {
    output << "\n";
  }

  return result;
}

std::string chernikov::parseInputToString(std::istream &input, ParseResult &result)
{
  std::ostringstream output;
  result = parseInput(input, output);
  return output.str();
}
