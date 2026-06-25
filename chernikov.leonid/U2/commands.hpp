#ifndef CHERNIKOV_COMMANDS_HPP
#define CHERNIKOV_COMMANDS_HPP

#include <string>
#include "hash_table.hpp"
#include "person.hpp"
#include "meets.hpp"

namespace chernikov
{

  using PersonTable = HashTable<std::size_t, Person *>;

  struct AppState
  {
    PersonTable persons;
    MeetsByPerson meets;
  };

  void initAppState(AppState &state);

  void destroyAppState(AppState &state);

  void loadPersonsFromFile(AppState &state, const std::string &filename);

  void loadMeetsFromFile(AppState &state, const std::string &filename);

  void executeCommand(AppState &state, const std::string &line);

  bool hasPerson(const AppState &state, std::size_t id);

  Person *getPerson(AppState &state, std::size_t id);

  void ensurePersonExists(AppState &state, std::size_t id);

}

#endif
