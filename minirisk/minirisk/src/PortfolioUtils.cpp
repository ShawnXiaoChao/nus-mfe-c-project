#include "Global.h"
#include "PortfolioUtils.h"
#include "TradePayment.h"



#include <numeric>
#include <math.h>

namespace minirisk {

	void print_portfolio(const portfolio_t& portfolio)
	{
		std::for_each(portfolio.begin(), portfolio.end(), [](auto& pt) { pt->print(std::cout); });
	}

	std::vector<ppricer_t> get_pricers(const portfolio_t& portfolio)
	{
		std::vector<ppricer_t> pricers(portfolio.size());
		std::transform(portfolio.begin(), portfolio.end(), pricers.begin()
			, [](auto& pt) -> ppricer_t { return pt->pricer(); });
		return pricers;
	}

	//MY PART starts+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	portfolio_values_t compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt)
	{
		portfolio_values_t prices;
		for (auto& pp : pricers)
		{
			try {
				auto comp_prices = pp->price(mkt);
				prices.push_back(std::make_pair(comp_prices, ""));
			}
			catch (std::exception& e) {
				prices.push_back(std::make_pair(std::numeric_limits<double>::quiet_NaN(), +e.what()));
			}
		}
		return prices;
	}

	compute_total_t portfolio_total(const portfolio_values_t& values)
	{
		compute_total_t trades_total;
		for (int i = 0; i < values.size(); ++i)
		{
			if (isnan(values[i].first))
				trades_total.second.push_back(std::make_pair(i, values[i].second));
			else
				trades_total.first += values[i].first;
		}
		return trades_total;
	}

	std::vector<std::pair<string, portfolio_values_t>> PV01Bucketed(const std::vector<ppricer_t>& pricers, const Market& mkt)
	{
		std::vector<std::pair<string, portfolio_values_t>> pv01;  // PV01 per trade

		const double bump_size = 0.01 / 100;

		// Filter risk factors related to IR.*.
		auto base = mkt.get_risk_factors(ir_rate_prefix + "\.[0-9]*[A-Z]\." + "[A-Z]{3}");

		// We Make a local copy of Market object, since we will modify it applying bumps.
		// Note that the actual market objects are referred to via pointers and are shared.
		Market tmpmkt(mkt);

		// Compute prices for perturbated markets and aggregate results.
		pv01.reserve(base.size());
		for (const auto& d : base) {
			//The code below changes the type of pv_up and pv_dn
			portfolio_values_t pv_up, pv_dn; 
			std::vector<std::pair<string, double>> bumped(1, d);
			pv01.push_back(std::make_pair(d.first, portfolio_values_t()));

			// Bump up and price.
			bumped[0].second = d.second + bump_size;
			tmpmkt.set_risk_factors(bumped);
			pv_up = compute_prices(pricers, tmpmkt);

			// Bump down and price.
			bumped[0].second = d.second - bump_size;
			tmpmkt.set_risk_factors(bumped);
			pv_dn = compute_prices(pricers, tmpmkt);

			// Restore original market state for next iteration. And note that it is 
			// more efficient than creating a new copy of the market at every iteration
			bumped[0].second = d.second;
			tmpmkt.set_risk_factors(bumped);

			// Compute estimator of the derivative by using central finite differences.
			double dr = 2.0 * bump_size;
			for (int i = 0; i < pv_up.size(); i++)
			{
				if (isnan(pv_up[i].first) || isnan(pv_dn[i].first))
					pv01.back().second.push_back(std::make_pair(std::numeric_limits<double>::quiet_NaN(), isnan(pv_up[i].first) ? pv_up[i].second : pv_dn[i].second));
				else
					pv01.back().second.push_back(std::make_pair((pv_up[i].first - pv_dn[i].first) / dr, ""));
			}
		}
		return pv01;
	}

	std::vector<std::pair<string, portfolio_values_t>> PV01Parallel(const std::vector<ppricer_t>& pricers, const Market& mkt)
	{
		// This is the PV01 per trade.
		std::vector<std::pair<string, portfolio_values_t>> pv01;  

		const double bump_size = (0.01 / 100);

		// Filter risk factors related to IR.*.
		auto base = mkt.get_risk_factors(ir_rate_prefix + "\.[0-9]*[A-Z]\." + "[A-Z]{3}");


		//Here need to make two copy of Market object because all risk factors 
		//move simutaneously for bumping and bumping down.
		Market tmpmkt(mkt);

		pv01.reserve(base.size());
		for (const auto& d : base) {
			portfolio_values_t pv_up, pv_dn;
			std::vector<std::pair<string, double>> bumped(1, d);
			pv01.push_back(std::make_pair(d.first, portfolio_values_t()));

			// Bump up.
			bumped[0].second = d.second + bump_size;
			tmpmkt.set_risk_factors(bumped);

			// Bump down.
			bumped[0].second = d.second - bump_size;
			tmpmkt.set_risk_factors(bumped);

			//compute prices
			pv_up = compute_prices(pricers, tmpmkt);
			pv_dn = compute_prices(pricers, tmpmkt);

			// Compute estimator of the derivative via central finite differences.
			double dr = 2.0 * bump_size;
			for (int i = 0; i < pv_up.size(); i++)
			{
				if (isnan(pv_up[i].first) || isnan(pv_dn[i].first))
					pv01.back().second.push_back(std::make_pair(std::numeric_limits<double>::quiet_NaN(), (isnan(pv_up[i].first) ? pv_up[i].second : pv_dn[i].second)));
				else
					pv01.back().second.push_back(std::make_pair((pv_up[i].first - pv_dn[i].first) / dr, ""));
			}
		}
		return pv01;
	}
	//MY PART ends++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	ptrade_t load_trade(my_ifstream& is)
	{
		string name;
		ptrade_t p;

		// read trade identifier
		guid_t id;
		is >> id;

		if (id == TradePayment::m_id)
			p.reset(new TradePayment);
		else
			THROW("Unknown trade type:" << id);

		p->load(is);

		return p;
	}

	void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio)
	{
		// test saving to file
		my_ofstream of(filename);
		for (const auto& pt : portfolio) {
			pt->save(of);
			of.endl();
		}
		of.close();
	}

	std::vector<ptrade_t> load_portfolio(const string& filename)
	{
		std::vector<ptrade_t> portfolio;

		// test reloading the portfolio
		my_ifstream is(filename);
		while (is.read_line())
			portfolio.push_back(load_trade(is));

		return portfolio;
	}

	void print_price_vector(const string& name, const portfolio_values_t& values)
	{
		std::cout
			<< "========================\n"
			<< name << ":\n"
			<< "========================\n"
			<< "Total: " << portfolio_total(values).first << "\n"
			<< "Errors: " << portfolio_total(values).second.size()
			<< "\n========================\n";

		for (size_t i = 0, n = values.size(); i < n; ++i)
			if (isnan(values[i].first))
				std::cout << std::setw(5) << i << ": " << values[i].second << "\n";
			else
				std::cout << std::setw(5) << i << ": " << values[i].first << "\n";

		std::cout << "========================\n\n";
	}

} // namespace minirisk
