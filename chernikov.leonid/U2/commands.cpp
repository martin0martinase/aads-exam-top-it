#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>

namespace
{

  bool isSpace(char c)
  {
    return c == ' ' || c == '\t';
  }

  bool compareMeets(const chernikov::Meet &a, const chernikov::Meet &b)
  {
    if (a.person2 != b.person2)
    {
      return a.person2 < b.person2;
    }
    return a.duration < b.duration;
  }

  void printInvalidCommand()
  {
    std::cout << "<INVALID COMMAND>\n";
  }

}

void chernikov::initAppState(AppState &state)
{
  initHashTable(state.persons);
  initMeets(state.meets);
}

void chernikov::destroyAppState(AppState &state)
{
  auto it = beginHashTable(state.persons);
  auto end = endHashTable(state.persons);
  for (; iteratorsNotEqual(it, end); advanceIterator(it))
  {
    delete it.currentNode->value;
  }
  destroyHashTable(state.persons);
  destroyMeets(state.meets);
}

void chernikov::loadPersonsFromFile(AppState &state, const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    return;
  }

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::size_t pos = 0;
    std::size_t len = line.size();

    while (pos < len && isSpace(line[pos]))
    {
      ++pos;
    }

    if (pos >= len || !std::isdigit(line[pos]))
    {
      continue;
    }

    std::size_t id = 0;
    while (pos < len && std::isdigit(line[pos]))
    {
      id = id * 10 + (line[pos] - '0');
      ++pos;
    }

    while (pos < len && isSpace(line[pos]))
    {
      ++pos;
    }

    Person *person = new Person();
    initPerson(*person, id);

    if (pos < len)
    {
      setDescription(*person, line.substr(pos));
    }

    if (hasInHashTable(state.persons, id))
    {
      delete getFromHashTable(state.persons, id);
    }
    addToHashTable(state.persons, id, person);
  }

  file.close();
}

void chernikov::loadMeetsFromFile(AppState &state, const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Failed to open meets file\n";
    std::exit(2);
  }

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::size_t id1 = 0;
    std::size_t id2 = 0;
    std::size_t duration = 0;
    std::size_t pos = 0;
    std::size_t len = line.size();

    while (pos < len && isSpace(line[pos]))
    {
      ++pos;
    }

    if (pos >= len || !std::isdigit(line[pos]))
    {
      std::cerr << "Invalid meet data\n";
      std::exit(3);
    }

    while (pos < len && std::isdigit(line[pos]))
    {
      id1 = id1 * 10 + (line[pos] - '0');
      ++pos;
    }

    while (pos < len && isSpace(line[pos]))
    {
      ++pos;
    }

    if (pos >= len || !std::isdigit(line[pos]))
    {
      std::cerr << "Invalid meet data\n";
      std::exit(3);
    }

    while (pos < len && std::isdigit(line[pos]))
    {
      id2 = id2 * 10 + (line[pos] - '0');
      ++pos;
    }

    while (pos < len && isSpace(line[pos]))
    {
      ++pos;
    }

    if (pos >= len || !std::isdigit(line[pos]))
    {
      std::cerr << "Invalid meet data\n";
      std::exit(3);
    }

    while (pos < len && std::isdigit(line[pos]))
    {
      duration = duration * 10 + (line[pos] - '0');
      ++pos;
    }

    if (id1 == id2)
    {
      continue;
    }

    ensurePersonExists(state, id1);
    ensurePersonExists(state, id2);

    Meet meet1{id1, id2, duration};
    addMeetToPerson(state.meets, id1, meet1);

    Meet meet2{id2, id1, duration};
    addMeetToPerson(state.meets, id2, meet2);
  }

  file.close();
}

void chernikov::executeCommand(AppState &state, const std::string &line)
{
  std::istringstream iss(line);
  std::string command;
  iss >> command;

  if (command == "anons")
  {
    std::vector<std::size_t> anons;
    auto it = beginHashTable(state.persons);
    auto end = endHashTable(state.persons);
    for (; iteratorsNotEqual(it, end); advanceIterator(it))
    {
      if (!hasDescription(*it.currentNode->value))
      {
        anons.push_back(it.currentNode->value->id);
      }
    }
    std::sort(anons.begin(), anons.end());
    for (std::size_t id : anons)
    {
      std::cout << id << "\n";
    }
    return;
  }

  if (command == "deanon")
  {
    std::size_t anonId = 0;
    std::size_t realId = 0;
    if (!(iss >> anonId >> realId))
    {
      printInvalidCommand();
      return;
    }

    if (!hasPerson(state, anonId) || !hasPerson(state, realId))
    {
      printInvalidCommand();
      return;
    }

    Person *anon = getPerson(state, anonId);
    Person *real = getPerson(state, realId);

    if (hasDescription(*anon) || !hasDescription(*real))
    {
      printInvalidCommand();
      return;
    }

    moveMeets(state.meets, anonId, realId);

    dropFromHashTable(state.persons, anonId);
    delete anon;

    return;
  }

  if (command == "redesc")
  {
    std::size_t id = 0;
    iss >> id;

    if (!hasPerson(state, id))
    {
      printInvalidCommand();
      return;
    }

    std::string rest;
    std::getline(iss, rest);

    std::size_t start = 0;
    while (start < rest.size() && isSpace(rest[start]))
    {
      ++start;
    }

    if (start >= rest.size() || rest[start] != '"')
    {
      printInvalidCommand();
      return;
    }

    std::size_t end = rest.size() - 1;
    while (end > start && rest[end] != '"')
    {
      --end;
    }

    if (end <= start)
    {
      printInvalidCommand();
      return;
    }

    std::string description = rest.substr(start + 1, end - start - 1);

    Person *person = getPerson(state, id);
    setDescription(*person, description);

    return;
  }

  if (command == "desc")
  {
    std::size_t id = 0;
    if (!(iss >> id))
    {
      printInvalidCommand();
      return;
    }

    if (!hasPerson(state, id))
    {
      printInvalidCommand();
      return;
    }

    Person *person = getPerson(state, id);
    if (hasDescription(*person))
    {
      std::cout << person->info << "\n";
    }
    else
    {
      std::cout << "<ANON>\n";
    }
    return;
  }

  if (command == "meets")
  {
    std::size_t id = 0;
    if (!(iss >> id))
    {
      printInvalidCommand();
      return;
    }

    if (!hasPerson(state, id))
    {
      printInvalidCommand();
      return;
    }

    MeetList *list = getMeets(state.meets, id);
    if (list == nullptr || list->count == 0)
    {
      return;
    }

    sortMeets(*list);

    for (std::size_t i = 0; i < list->count; ++i)
    {
      std::cout << list->meets[i].person2 << " " << list->meets[i].duration << "\n";
    }
    return;
  }

  if (command == "commons")
  {
    std::size_t id1 = 0;
    std::size_t id2 = 0;
    if (!(iss >> id1 >> id2))
    {
      printInvalidCommand();
      return;
    }

    if (!hasPerson(state, id1) || !hasPerson(state, id2))
    {
      printInvalidCommand();
      return;
    }

    MeetList *list1 = getMeets(state.meets, id1);
    MeetList *list2 = getMeets(state.meets, id2);

    std::vector<std::size_t> common;

    if (list1 != nullptr && list2 != nullptr)
    {
      for (std::size_t i = 0; i < list1->count; ++i)
      {
        std::size_t otherId = list1->meets[i].person2;
        bool found = false;
        for (std::size_t j = 0; j < list2->count; ++j)
        {
          if (list2->meets[j].person2 == otherId)
          {
            found = true;
            break;
          }
        }
        if (found)
        {
          bool alreadyAdded = false;
          for (std::size_t k = 0; k < common.size(); ++k)
          {
            if (common[k] == otherId)
            {
              alreadyAdded = true;
              break;
            }
          }
          if (!alreadyAdded)
          {
            common.push_back(otherId);
          }
        }
      }
    }

    std::sort(common.begin(), common.end());
    for (std::size_t id : common)
    {
      std::cout << id << "\n";
    }
    return;
  }

  if (command == "less")
  {
    std::size_t time = 0;
    std::size_t id = 0;
    if (!(iss >> time >> id))
    {
      printInvalidCommand();
      return;
    }

    if (!hasPerson(state, id))
    {
      printInvalidCommand();
      return;
    }

    MeetList *list = getMeets(state.meets, id);
    if (list == nullptr)
    {
      return;
    }

    std::vector<Meet> filtered;
    for (std::size_t i = 0; i < list->count; ++i)
    {
      if (list->meets[i].duration < time)
      {
        filtered.push_back(list->meets[i]);
      }
    }

    std::sort(filtered.begin(), filtered.end(), compareMeets);

    for (const auto &meet : filtered)
    {
      std::cout << meet.person2 << " " << meet.duration << "\n";
    }
    return;
  }

  if (command == "greater")
  {
    std::size_t time = 0;
    std::size_t id = 0;
    if (!(iss >> time >> id))
    {
      printInvalidCommand();
      return;
    }

    if (!hasPerson(state, id))
    {
      printInvalidCommand();
      return;
    }

    MeetList *list = getMeets(state.meets, id);
    if (list == nullptr)
    {
      return;
    }

    std::vector<Meet> filtered;
    for (std::size_t i = 0; i < list->count; ++i)
    {
      if (list->meets[i].duration > time)
      {
        filtered.push_back(list->meets[i]);
      }
    }

    std::sort(filtered.begin(), filtered.end(), compareMeets);

    for (const auto &meet : filtered)
    {
      std::cout << meet.person2 << " " << meet.duration << "\n";
    }
    return;
  }

  if (command == "out-persons")
  {
    std::string filename;
    if (!(iss >> filename))
    {
      printInvalidCommand();
      return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
      printInvalidCommand();
      return;
    }

    std::vector<Person *> personsWithDesc;
    auto it = beginHashTable(state.persons);
    auto end = endHashTable(state.persons);
    for (; iteratorsNotEqual(it, end); advanceIterator(it))
    {
      if (hasDescription(*it.currentNode->value))
      {
        personsWithDesc.push_back(it.currentNode->value);
      }
    }

    std::sort(personsWithDesc.begin(), personsWithDesc.end(),
              [](const Person *a, const Person *b)
              {
                return a->id < b->id;
              });

    for (const Person *person : personsWithDesc)
    {
      outFile << person->id << " " << person->info << "\n";
    }

    outFile.close();
    return;
  }

  printInvalidCommand();
}

bool chernikov::hasPerson(const AppState &state, std::size_t id)
{
  return hasInHashTable(state.persons, id);
}

chernikov::Person *chernikov::getPerson(AppState &state, std::size_t id)
{
  return getFromHashTable(state.persons, id);
}

void chernikov::ensurePersonExists(AppState &state, std::size_t id)
{
  if (!hasInHashTable(state.persons, id))
  {
    Person *person = new Person();
    initPerson(*person, id);
    addToHashTable(state.persons, id, person);
  }
}
