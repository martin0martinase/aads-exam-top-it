#include "date.hpp"
#include <ostream>

bool chernikov::datesEqual(const Date &a, const Date &b)
{
  return a.year == b.year && a.month == b.month && a.day == b.day;
}

bool chernikov::dateLess(const Date &a, const Date &b)
{
  if (a.year != b.year)
  {
    return a.year < b.year;
  }
  if (a.month != b.month)
  {
    return a.month < b.month;
  }
  return a.day < b.day;
}

bool chernikov::dateLessOrEqual(const Date &a, const Date &b)
{
  return dateLess(a, b) || datesEqual(a, b);
}

bool chernikov::dateGreater(const Date &a, const Date &b)
{
  return dateLess(b, a);
}

bool chernikov::dateGreaterOrEqual(const Date &a, const Date &b)
{
  return dateLessOrEqual(b, a);
}

void chernikov::printDate(const Date &date, std::ostream &out)
{
  out << date.day << " " << date.month << " " << date.year;
}
