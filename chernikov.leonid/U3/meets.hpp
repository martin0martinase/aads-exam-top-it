#ifndef CHERNIKOV_MEETS_HPP
#define CHERNIKOV_MEETS_HPP

#include <cstddef>
#include "hash_table.hpp"
#include "date.hpp"

namespace chernikov
{

  struct Meet
  {
    std::size_t person1;
    std::size_t person2;
    std::size_t duration;
  };

  struct MeetList
  {
    std::size_t capacity;
    std::size_t count;
    Meet *meets;
  };

  using MeetsByPerson = HashTable<std::size_t, MeetList *>;
  using MeetsByDate = HashTable<std::size_t, MeetsByPerson *>;

  void initMeetList(MeetList &list);

  void destroyMeetList(MeetList &list);

  void addMeet(MeetList &list, const Meet &meet);

  void sortMeets(MeetList &list);

  void initMeetsByPerson(MeetsByPerson &meets);

  void destroyMeetsByPerson(MeetsByPerson &meets);

  void addMeetToPerson(MeetsByPerson &meets, std::size_t personId, const Meet &meet);

  MeetList *getMeetsByPerson(MeetsByPerson &meets, std::size_t personId);

  void removeMeetsFromPerson(MeetsByPerson &meets, std::size_t fromId, std::size_t toId);

  void moveMeets(MeetsByPerson &meets, std::size_t fromId, std::size_t toId);

}

#endif
