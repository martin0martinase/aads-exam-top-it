#ifndef CHERNIKOV_DATE_HPP
#define CHERNIKOV_DATE_HPP

#include <cstddef>
#include <iostream>

namespace chernikov
{

  struct Date
  {
    std::size_t day;
    std::size_t month;
    std::size_t year;
  };

  bool datesEqual(const Date &a, const Date &b);

  bool dateLess(const Date &a, const Date &b);

  bool dateLessOrEqual(const Date &a, const Date &b);

  bool dateGreater(const Date &a, const Date &b);

  bool dateGreaterOrEqual(const Date &a, const Date &b);

  void printDate(const Date &date, std::ostream &out);

}

#endif
