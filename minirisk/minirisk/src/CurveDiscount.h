#pragma once
#include "ICurve.h"
#include <vector>
#include <map>


namespace minirisk {

    struct Market;

    struct CurveDiscount : ICurveDiscount
    {
        virtual string name() const { return m_name; }

        CurveDiscount(Market *mkt, const Date& today, const string& curve_name);

        // compute the discount factor
        double df(const Date& t) const;

        virtual Date today() const { return m_today; }

    private:
        Date   m_today;
        string m_name;
		std::vector<Date> m_date;  //save the date points
		std::vector<double> m_rate_t; //save the rate of correspoding date points
	    //std::map <Date, double> m_rate; //save map to change m_rate   
		double m_rate;
    };

} // namespace minirisk
