#ifndef CHERNIKOV_DATE_RANGE_HPP
#define CHERNIKOV_DATE_RANGE_HPP

#include "date.hpp"
#include "hash_table.hpp"

namespace chernikov
{

  struct DateRange
  {
    Date minDate;
    Date maxDate;
    bool isEmpty;
  };

  struct RangeStack
  {
    std::size_t capacity;
    std::size_t count;
    DateRange *ranges;
  };

  using DateSet = HashTable<std::size_t, bool>;

  void initDateRange(DateRange &range);

  void initRangeStack(RangeStack &stack);

  void destroyRangeStack(RangeStack &stack);

  void pushRange(RangeStack &stack, const DateRange &range);

  bool popRange(RangeStack &stack, DateRange &range);

  void addDate(DateSet &dates, const Date &date);

  void buildDateSet(DateSet &dates, const DateRange &range);

  bool isDateInRange(const Date &date, const DateRange &range);

}

#endif
