#include "FixingDataServer.h"
#include "Macros.h"
#include "Streamer.h"

#include <limits>

namespace minirisk {

FixingDataServer::FixingDataServer(const std::string& filename)
{
    std::ifstream is(filename);
    MYASSERT(!is.fail(), "Could not open file " << filename);
    do {
        string name;
        string date;
        double value;
        unsigned y, m, d;
        is >> name >> date >> value;
        
        y = std::stoul(date.substr(0, 4));
        m = std::stoul(date.substr(4, 2));
        d = std::stoul(date.substr(6, 2));
        Date m_date(y, m, d);
        
        m_fixings.emplace(name, std::map<Date, double>());
        auto ins = m_fixings[name].emplace(m_date, value);
        MYASSERT(ins.second, "Duplicated fixing: " << date << " " << value);
    } while (is);
}

double FixingDataServer::get(const string& name, const Date& t) const
{
    auto iter_name = m_fixings.find(name);
    MYASSERT(iter_name != m_fixings.end(), "Fixing not found: " << name << "," << t);
    auto iter_date = iter_name->second.find(t);
    MYASSERT(iter_date != iter_name->second.end(), "Fixing not found: " << name << ", " << t);
    return iter_date->second;
}

std::pair<double, bool> FixingDataServer::lookup(const string& name, const Date& t) const
{
    auto iter_name = m_fixings.find(name);
    if (iter_name == m_fixings.end())
        return (std::make_pair(std::numeric_limits<double>::quiet_NaN(), false));
    else
    {
        auto iter_date = iter_name->second.find(t);
        return (iter_date != iter_name->second.end())
            ? std::make_pair(iter_date->second, true)
            : std::make_pair(std::numeric_limits<double>::quiet_NaN(), false);
    }
}

} // namespace minirisk

