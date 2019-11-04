#pragma once

#include <vector>

#include "ITrade.h"
#include "IPricer.h"

namespace minirisk {

	struct Market;
	// MY PART starts+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	typedef std::vector<std::pair<double, string>> portfolio_values_t;
	typedef std::pair<double, std::vector<std::pair<size_t, string>>> compute_total_t;
	//MY PART ends+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// get pricer for each trade
	std::vector<ppricer_t> get_pricers(const portfolio_t& portfolio);

	// compute prices
	portfolio_values_t compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt);

	// compute the cumulative book value
	compute_total_t portfolio_total(const portfolio_values_t& values); //MY PART modified here+++++++++++++++++++++++++++++++++

	// Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
	// Use central differences, absolute bump of 0.01%, rescale result for rate movement of 0.01%
	std::vector<std::pair<string, portfolio_values_t>> PV01Bucketed(const std::vector<ppricer_t>& pricers, const Market& mkt); //Modified++++++++++++++++++++
	std::vector<std::pair<string, portfolio_values_t>> PV01Parallel(const std::vector<ppricer_t>& pricers, const Market& mkt); //Modified++++++++++++++++++++

	// save portfolio to file
	void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio);

	// load portfolio from file
	std::vector<ptrade_t>  load_portfolio(const string& filename);

	// print portfolio to cout
	void print_portfolio(const portfolio_t& portfolio);

	// print portfolio to cout
	void print_price_vector(const string& name, const portfolio_values_t& values);


} // namespace minirisk

