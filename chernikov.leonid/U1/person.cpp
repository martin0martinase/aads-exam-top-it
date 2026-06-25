#include "person.hpp"
#include <ostream>

bool chernikov::isValidPerson(const Person &person)
{
  return !person.info.empty();
}

void chernikov::printPerson(const Person &person, std::ostream &out)
{
  out << person.id << " " << person.info << "\n";
}
