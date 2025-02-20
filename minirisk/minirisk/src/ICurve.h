#pragma once

#include <memory>
#include <string>

#include "IObject.h"
#include "Date.h"
#include<map>
using std::string;

namespace minirisk {

// forward declarations
struct Market;
struct MarketDataServer;

struct ICurve : IObject
{
    virtual string name() const = 0;
    virtual Date today() const = 0;
};

// forward declaration
struct ICurveDiscount;
struct ICurveFXSpot;
struct ICurveFXForward;

typedef std::shared_ptr<const ICurve> ptr_curve_t;
typedef std::shared_ptr<const ICurveDiscount> ptr_disc_curve_t;
typedef std::shared_ptr<const ICurveFXSpot> ptr_fx_spot_curve_t;
typedef std::shared_ptr<const ICurveFXForward> ptr_fx_forward_curve_t;

struct ICurveDiscount : ICurve
{
    // compute the discount factor for date t
    virtual std::pair<double, unsigned> df(const Date& t) const = 0;
};

struct ICurveFXForward : ICurve
{
    // compute the FX forward price of currency ccy1 deniminated in ccy2 for delivery at time t
    virtual double fwd(const Date& t) const = 0;
};

struct ICurveFXSpot : ICurve
{
    // return the FX spot price of currency ccy1 deniminated in ccy2 for delivery at time t
    // ignore the fact that the FX spot is a t+2 forward price itself, and assume it is the istantaneous exchange rate
    virtual double get_spot(const std::string& name) const = 0;
};

} // namespace minirisk

