#include "date_range.hpp"

namespace
{

  std::size_t dateToKey(const chernikov::Date &date)
  {
    return date.year * 10000 + date.month * 100 + date.day;
  }

  chernikov::Date keyToDate(std::size_t key)
  {
    chernikov::Date date;
    date.year = key / 10000;
    date.month = (key / 100) % 100;
    date.day = key % 100;
    return date;
  }

}

void chernikov::initDateRange(DateRange &range)
{
  range.isEmpty = true;
}

void chernikov::initRangeStack(RangeStack &stack)
{
  stack.capacity = 4;
  stack.count = 0;
  stack.ranges = new DateRange[stack.capacity];
}

void chernikov::destroyRangeStack(RangeStack &stack)
{
  delete[] stack.ranges;
  stack.ranges = nullptr;
  stack.count = 0;
  stack.capacity = 0;
}

void chernikov::pushRange(RangeStack &stack, const DateRange &range)
{
  if (stack.count >= stack.capacity)
  {
    std::size_t newCapacity = stack.capacity * 2;
    DateRange *newRanges = new DateRange[newCapacity];
    for (std::size_t i = 0; i < stack.count; ++i)
    {
      newRanges[i] = stack.ranges[i];
    }
    delete[] stack.ranges;
    stack.ranges = newRanges;
    stack.capacity = newCapacity;
  }
  stack.ranges[stack.count] = range;
  ++stack.count;
}

bool chernikov::popRange(RangeStack &stack, DateRange &range)
{
  if (stack.count == 0)
  {
    return false;
  }
  --stack.count;
  range = stack.ranges[stack.count];
  return true;
}

void chernikov::addDate(DateSet &dates, const Date &date)
{
  std::size_t key = dateToKey(date);
  addToHashTable(dates, key, true);
}

void chernikov::buildDateSet(DateSet &dates, const DateRange &range)
{
  initHashTable(dates);

  if (range.isEmpty)
  {
    return;
  }

  std::size_t startKey = dateToKey(range.minDate);
  std::size_t endKey = dateToKey(range.maxDate);

  for (std::size_t key = startKey; key <= endKey; ++key)
  {
    Date date = keyToDate(key);
    if (isDateInRange(date, range))
    {
      addToHashTable(dates, key, true);
    }
  }
}

bool chernikov::isDateInRange(const Date &date, const DateRange &range)
{
  if (range.isEmpty)
  {
    return false;
  }
  return dateGreaterOrEqual(date, range.minDate) && dateLessOrEqual(date, range.maxDate);
}
