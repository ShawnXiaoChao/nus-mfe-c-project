#include "CurveDiscount.h"
#include "Market.h"
#include "Streamer.h"

#include <cmath>


namespace minirisk {

	CurveDiscount::CurveDiscount(Market *mkt, const Date& today, const string& curve_name)
		: m_today(today)
		, m_name(curve_name)
		, m_rate(mkt->get_yield(curve_name.substr(ir_curve_discount_prefix.length(), 3)))
	{
	}

	double  CurveDiscount::df(const Date& t) const
	{
		// called with a date beyond the last tenor or before the anchor date (today), generate an error.
		MYASSERT((!(t < m_today)), "Curve " << m_name << ", DF is not available before the anchor date " << m_today << ", requested " << t);
		MYASSERT((!(t > m_date.back())), "Curve " << m_name << ", DF is not available beyond the last tenor date " << m_date.back() << ", requested " << t);

		std::vector<Date>::const_iterator low;
		low = std::lower_bound(m_date.begin(), m_date.end(), t);//binary search
		if (low != m_date.begin() && !(*low == t))//call with the date which is between two date in data
		{
			double local_rate = (m_rate_t[low - m_date.begin()] - m_rate_t[low - m_date.begin() - 1]) / (time_frac(*(low - 1), *low));
			return exp(-m_rate_t[low - m_date.begin() - 1]) * exp(-local_rate * time_frac(*(low - 1), t));
		}
		else  //call with the date which is the date in data
		{
			double local_rate = m_rate_t[low - m_date.begin()] / (time_frac(m_today, *low));
			return exp(-local_rate * time_frac(m_today, t));
		}
	}

} // namespace minirisk
