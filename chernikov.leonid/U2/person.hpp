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
    bool hasDescription;
  };

  void initPerson(Person &person, std::size_t id);

  void setDescription(Person &person, const std::string &description);

  bool hasDescription(const Person &person);

  void printPerson(const Person &person, std::ostream &out);

}

#endif
