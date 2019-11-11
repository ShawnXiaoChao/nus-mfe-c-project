#pragma once

#include <memory>

#include "IObject.h"
#include "Market.h"
#include "FixingDataServer.h"

namespace minirisk {

struct IPricer : IObject
{
    virtual std::pair<double, string> price(Market& mkt) const { return price(mkt, nullptr); }
    virtual std::pair<double, string> price(Market& mkt, FixingDataServer* fds) const = 0;
};


typedef std::shared_ptr<const IPricer> ppricer_t;

} // namespace minirisk
