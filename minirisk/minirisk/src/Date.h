#pragma once

#include "Macros.h"
#include <string>
#include <array>

namespace minirisk {

struct Date
{
public:
    static const unsigned first_year = 1900;
    static const unsigned last_year = 2200;
    static const unsigned n_years = last_year - first_year;

private:
    static std::string padding_dates(unsigned);

    // number of days elapsed from beginning of the year
    unsigned day_of_year() const;

    friend long operator-(const Date& d1, const Date& d2);

    static const std::array<unsigned, 12> days_in_month;  // num of days in month M in a normal year
    static const std::array<unsigned, 12> days_ytd;      // num of days since 1-jan to 1-M in a normal year
	static const std::array<unsigned, 12> days_ytd_leap; //num of days since 1-jan to 1-M in a leap year
    static const std::array<unsigned, n_years> days_epoch;   // num of days since 1-jan-1900 to 1-jan-yyyy (until 2200)

public:
    // Default constructor
	Date() : m_serial(0) {}
	Date(unsigned serial) :m_serial(serial) {}
	Date(const std::string& yyyymmdd);

    // Constructor where the input value is checked.
    Date(unsigned year, unsigned month, unsigned day)
    {
		check_valid(year, month, day);
		m_serial = days_epoch[year - 1900] + 
			((is_leap_year(year) ? (days_ytd[month - 1] + 1) : days_ytd[month - 1]) 
				+ day - 1);
		//transer year,month,day into serial
    }

    void init(unsigned serial)
    {
		m_serial =(unsigned) serial;
    }

    static void check_valid(unsigned y, unsigned m, unsigned d);

    bool operator<(const Date& d) const
    {
        return m_serial<d.serial();
    }

    bool operator==(const Date& d) const
    {
        return m_serial == d.serial();
    }

    bool operator>(const Date& d) const
    {
        return m_serial > d.serial();
    }

	Date operator+(const int days) const {
		return Date(serial() + days);
	}
	Date operator-(const int days) const {
		return Date(serial() - days);
	}
			
	 // number of days since 1-Jan-1900
    unsigned serial() const
    {
		return m_serial;
    }

    static bool is_leap_year(unsigned yr);

	void to_year_month_day(unsigned* y, unsigned* m, unsigned* d) const;

    // In YYYYMMDD format
    std::string to_string(bool pretty = true) const
    {
		unsigned m_d, m_m, m_y;
		to_year_month_day(&m_y,&m_m,&m_d);

        return pretty
            ? std::to_string((int)m_d) + "-" + std::to_string((int)m_m) + "-" + std::to_string(m_y)
            : std::to_string(m_y) + padding_dates((int)m_m) + padding_dates((int)m_d);
    }

private:
  
	unsigned m_serial;
};

long operator-(const Date& d1, const Date& d2);

inline double time_frac(const Date& d1, const Date& d2)
{
    return static_cast<double>(d2 - d1) / 365.0;
}

} // namespace minirisk
