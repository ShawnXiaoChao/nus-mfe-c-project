#include "CurveFXSpot.h"
#include "Market.h"

namespace minirisk {

	unsigned tmp_count = 0;
CurveFXSpot::CurveFXSpot(
    Market *mkt, const Date& today, const std::string& name) 
    : m_today(today), m_name(name), fx_map(mkt->get_fx_spot(name)) {
	std::cout << fx_map.begin()->first << fx_map.begin()->second << std::endl;
	tmp_count += 1;
	std::cout << tmp_count << std::endl;
}
} // namespace minirisk