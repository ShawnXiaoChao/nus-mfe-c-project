#include <iomanip>

#include "Date.h"
#include <string>

namespace minirisk {

struct DateInitializer : std::array<unsigned, Date::n_years>
{
    DateInitializer()
    {
        for (unsigned i = 0, s = 0, y = Date::first_year; i < size(); ++i, ++y) {
            (*this)[i] = s;
            s += 365 + (Date::is_leap_year(y) ? 1 : 0);
        }
    }
};

const std::array<unsigned, 12> Date::days_in_month = { {31,28,31,30,31,30,31,31,30,31,30,31} };
const std::array<unsigned, 12> Date::days_ytd{ {0,31,59,90,120,151,181,212,243,273,304,334} };
const std::array<unsigned, 12> Date::days_ytd_leap{ {0,31,60,91,121,152,182,213,244,274,305,335} };
const std::array<unsigned, Date::n_years> Date::days_epoch(static_cast<const std::array<unsigned, Date::n_years>&>(DateInitializer()));

Date::Date(const std::string& yyyymmdd) :
	Date(std::stoul(yyyymmdd.substr(0, 4)), 
		std::stoul(yyyymmdd.substr(4, 2)), 
		std::stoul(yyyymmdd.substr(6, 2))) {}

/* The function checks if a given year is a leap year.
    Leap year must be a multiple of 4, but it cannot be a multiple of 100 without also being a multiple of 400.
*/
bool Date::is_leap_year(unsigned year)
{
    return ((year % 4 != 0) ? false : 
		(year % 100 != 0) ? true : (year % 400 != 0) ? false : true);
}

// The function pads a zero before the month or day if it has only one digit.
std::string Date::padding_dates(unsigned month_or_day)
{
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << month_or_day;
    return os.str();
}

void Date::check_valid(unsigned y, unsigned m, unsigned d)
{
    MYASSERT(y >= first_year, "The year must be no earlier than year " << first_year << ", got " << y);
    MYASSERT(y < last_year, "The year must be smaller than year " << last_year << ", got " << y);
    MYASSERT(m >= 1 && m <= 12, "The month must be a integer between 1 and 12, got " << m);
    unsigned dmax = days_in_month[m - 1] + ((m == 2 && is_leap_year(y)) ? 1 : 0);
    MYASSERT(d >= 1 && d <= dmax, "The day must be a integer between 1 and " << dmax << ", got " << d);
}


void Date::to_year_month_day(unsigned* y, unsigned* m, unsigned* d) const
{
	unsigned left_days=m_serial;
	for (std::size_t i = 0; i < days_epoch.size(); i++)
		if (days_epoch[i] > m_serial)
		{
			*y = i - 1 + 1900;
			break;
		}
	//left_days -=days_epoch[*y - 1];
	if (is_leap_year(*y))
	{
		for (std::size_t i = 0; i < days_ytd_leap.size(); i++)
			if (days_ytd_leap[i] > left_days)
			{
				*m = i;
				break;
			}
	}
	else
	{
		for (std::size_t i = 0; i < days_ytd.size(); i++)
			if (days_ytd[i] > left_days)
			{
				*m = i;
				break;
			}
	}
	//left_days -= days_ytd[*m - 1];
	*d = left_days+1;
}
unsigned Date::day_of_year() const
{
	unsigned m_year;
	unsigned m_m;
	unsigned m_d;
	to_year_month_day(&m_year, &m_m, &m_d);
	bool m_is_leap = is_leap_year(m_year);
	return days_ytd[ m_m- 1] + ((m_m > 2 && m_is_leap) ? 1 : 0) + (m_d - 1);
}


/*  The function calculates the distance between two Dates.
    d1 > d2 is allowed, which returns the negative of d2-d1.
*/
long operator-(const Date& d1, const Date& d2)
{
    unsigned s1 = d1.serial();
    unsigned s2 = d2.serial();
    return static_cast<long>(s1) - static_cast<long>(s2);
}



} // namespace minirisk

