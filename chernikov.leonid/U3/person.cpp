#include "person.hpp"
#include <ostream>

void chernikov::initPerson(Person &person, std::size_t id)
{
  person.id = id;
  person.info = "";
  person.hasDescription = false;
}

void chernikov::setDescription(Person &person, const std::string &description)
{
  person.info = description;
  person.hasDescription = true;
}

bool chernikov::hasDescription(const Person &person)
{
  return person.hasDescription;
}

void chernikov::printPerson(const Person &person, std::ostream &out)
{
  out << person.id;
  if (person.hasDescription)
  {
    out << " " << person.info;
  }
  out << "\n";
}
