#include "CurveFXSpot.h"
#include "Market.h"


namespace minirisk {
  
  CurveFXSpot::CurveFXSpot(Market *mkt, const Date& today, const string& curve_name)
  
  : m_today(today)
  , m_name(curve_name) 
  , m_fxspot(mkt->get_fx_spot(curve_name)) 
  {}
 
} // namespace minirisk
