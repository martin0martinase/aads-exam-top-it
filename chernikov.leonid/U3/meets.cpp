#include "meets.hpp"
#include <algorithm>

namespace
{

  bool compareMeets(const chernikov::Meet &a, const chernikov::Meet &b)
  {
    if (a.person2 != b.person2)
    {
      return a.person2 < b.person2;
    }
    return a.duration < b.duration;
  }

}

void chernikov::initMeetList(MeetList &list)
{
  list.capacity = 4;
  list.count = 0;
  list.meets = new Meet[list.capacity];
}

void chernikov::destroyMeetList(MeetList &list)
{
  delete[] list.meets;
  list.meets = nullptr;
  list.count = 0;
  list.capacity = 0;
}

void chernikov::addMeet(MeetList &list, const Meet &meet)
{
  if (list.count >= list.capacity)
  {
    std::size_t newCapacity = list.capacity * 2;
    Meet *newMeets = new Meet[newCapacity];
    for (std::size_t i = 0; i < list.count; ++i)
    {
      newMeets[i] = list.meets[i];
    }
    delete[] list.meets;
    list.meets = newMeets;
    list.capacity = newCapacity;
  }
  list.meets[list.count] = meet;
  ++list.count;
}

void chernikov::sortMeets(MeetList &list)
{
  std::sort(list.meets, list.meets + list.count, compareMeets);
}

void chernikov::initMeetsByPerson(MeetsByPerson &meets)
{
  initHashTable(meets);
}

void chernikov::destroyMeetsByPerson(MeetsByPerson &meets)
{
  auto it = beginHashTable(meets);
  auto end = endHashTable(meets);
  for (; iteratorsNotEqual(it, end); advanceIterator(it))
  {
    destroyMeetList(*it.currentNode->value);
    delete it.currentNode->value;
  }
  destroyHashTable(meets);
}

void chernikov::addMeetToPerson(MeetsByPerson &meets, std::size_t personId, const Meet &meet)
{
  if (!hasInHashTable(meets, personId))
  {
    MeetList *list = new MeetList();
    initMeetList(*list);
    addToHashTable(meets, personId, list);
  }
  MeetList *list = getFromHashTable(meets, personId);
  addMeet(*list, meet);
}

chernikov::MeetList *chernikov::getMeetsByPerson(MeetsByPerson &meets, std::size_t personId)
{
  if (!hasInHashTable(meets, personId))
  {
    return nullptr;
  }
  return getFromHashTable(meets, personId);
}

void chernikov::removeMeetsFromPerson(MeetsByPerson &meets, std::size_t fromId, std::size_t toId)
{
  if (!hasInHashTable(meets, fromId))
  {
    return;
  }

  MeetList *list = getFromHashTable(meets, fromId);
  std::size_t writePos = 0;
  for (std::size_t i = 0; i < list->count; ++i)
  {
    if (list->meets[i].person2 != toId)
    {
      list->meets[writePos] = list->meets[i];
      ++writePos;
    }
  }
  list->count = writePos;
}

void chernikov::moveMeets(MeetsByPerson &meets, std::size_t fromId, std::size_t toId)
{
  if (!hasInHashTable(meets, fromId))
  {
    return;
  }

  MeetList *fromList = getFromHashTable(meets, fromId);
  for (std::size_t i = 0; i < fromList->count; ++i)
  {
    Meet meet = fromList->meets[i];
    if (meet.person2 == toId)
    {
      continue;
    }
    addMeetToPerson(meets, toId, meet);

    if (hasInHashTable(meets, meet.person2))
    {
      MeetList *otherList = getFromHashTable(meets, meet.person2);
      for (std::size_t j = 0; j < otherList->count; ++j)
      {
        if (otherList->meets[j].person2 == fromId)
        {
          Meet updatedMeet = otherList->meets[j];
          updatedMeet.person1 = toId;
          updatedMeet.person2 = toId;
          addMeetToPerson(meets, toId, updatedMeet);
        }
      }
      removeMeetsFromPerson(meets, meet.person2, fromId);
    }
  }

  destroyMeetList(*fromList);
  delete fromList;
  dropFromHashTable(meets, fromId);
}
