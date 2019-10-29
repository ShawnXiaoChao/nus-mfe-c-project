#pragma once
#include "ICurve.h"

namespace minirisk {

struct Market;

struct CurveFXForward : ICurveFXForward
{
    virtual string name() const { return m_name; }
    virtual Date today() const { return m_today; }

    CurveFXForward(Market *mkt, const Date& today, const string& curve_name);

    // get forward price
    double fwd(const Date& t) const;

private:
    Date   m_today;
    string m_name;
    ptr_spot_curve_t m_spot;
    ptr_disc_curve_t m_df_ccy1;
    ptr_disc_curve_t m_df_ccy2;
};


} // namespace minirisk

