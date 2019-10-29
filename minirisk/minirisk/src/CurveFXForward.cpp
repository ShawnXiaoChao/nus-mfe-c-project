#include "CurveFXForward.h"
#include "Market.h"
#include "Streamer.h"

#include <cmath>


namespace minirisk {

CurveFXForward::CurveFXForward(Market *mkt, const Date& today, const string& curve_name)
    : m_today(today)
    , m_name(curve_name)
    , m_spot(mkt->get_spot_curve(fx_spot_name(m_name.substr(m_name.length() - 7, 3), m_name.substr(m_name.length() - 3, 3))))
	, m_df_ccy1(mkt->get_discount_curve(ir_curve_discount_name(m_name.substr(m_name.length() - 7, 3))))
	, m_df_ccy2(mkt->get_discount_curve(ir_curve_discount_name(m_name.substr(m_name.length() - 3, 3))))
{
}

double CurveFXForward::fwd(const Date& t) const
{
    MYASSERT((!(t < m_today)), "cannot get forward price for date in the past: " << t);
    return m_spot->spot() * m_df_ccy1->df(t) / (m_df_ccy2->df(t));
}

} // namespace minirisk

