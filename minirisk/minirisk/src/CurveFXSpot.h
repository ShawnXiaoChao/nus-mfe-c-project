#pragma once
#include "ICurve.h"

namespace minirisk {

struct Market;

struct CurveFXSpot : ICurveFXSpot
{

    
    virtual Date today() const { return m_today; }

    virtual string name() const { return m_name; }

    CurveFXSpot(Market *mkt, const Date& today, const string& curve_name);

    double spot() const { return m_spot; }

   

private:
    Date   m_today;
    string m_name;
    double m_spot;
};


} // namespace minirisk

