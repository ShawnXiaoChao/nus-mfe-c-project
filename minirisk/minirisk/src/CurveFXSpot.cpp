#include "CurveFXSpot.h"
#include "Market.h"


namespace minirisk {
  
  CurveFXSpot::CurveFXSpot(Market *mkt, const Date& today, const std::string& curve_name)
  
  : m_today(today)
  , m_name(curve_name) 
  , m_rate(mkt->get_fx_spot(curve_name)) 
  {}
 
} // namespace minirisk
