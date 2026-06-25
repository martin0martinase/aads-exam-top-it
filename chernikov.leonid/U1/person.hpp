#ifndef CHERNIKOV_PERSON_HPP
#define CHERNIKOV_PERSON_HPP

#include <string>
#include <cstddef>
#include <iosfwd>

namespace chernikov
{

  struct Person
  {
    std::size_t id;
    std::string info;
  };

  bool isValidPerson(const Person &person);

  void printPerson(const Person &person, std::ostream &out);

}

#endif
