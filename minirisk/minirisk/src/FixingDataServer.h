#pragma once

#include <map>
#include "Date.h"
#include "Global.h"

namespace minirisk {

struct FixingDataServer
{
public:
    explicit FixingDataServer(const string& filename);

    double get(const string& name, const Date& t) const;
    std::pair < double, bool > lookup(const string& name, const Date& t) const;

private:
    std::map<string, std::map<Date,double>> m_fixings;
};
} // namespace minirisk
