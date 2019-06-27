#include "BarrierOption.h"
#include "EquityPriceGenerator.h"
#include "ResultSet.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <future>
#include <ctime>
#include <limits>
#include <cassert>

using std::vector;
using std::map;
using std::max;			// <algorithm>
using std::transform;
using std::for_each;
using std::exp;
using std::accumulate;
using std::async;
using std::future;
using std::clock_t;
using std::all_of;
using std::find_if;
using std::back_inserter;

BarrierOption::BarrierOption(double barrierLevel, double strike, double spot, double riskFreeRate, double volatility,
	double quantity, Barrier BarrierType, const Date& valueDate, const Date& expiryDate,
	const Date& settlementDate, unsigned numTimeSteps, unsigned numScenarios, bool runParallel,
	int seed, double greekShift, const Act365& dc) :barrierLevel_(barrierLevel),strike_(strike), spot_(spot),
	riskFreeRate_(riskFreeRate), volatility_(volatility), quantity_(quantity),
	BarrierType_(BarrierType), numTimeSteps_(numTimeSteps), numScenarios_(numScenarios), runParallel_(runParallel), seed_(seed),
	greekShift_(greekShift), tau_(dc.yearFraction(valueDate, expiryDate)), settlement_(dc.yearFraction(valueDate, settlementDate))
{
	calculate_();
}

OptionResults BarrierOption::operator()() const
{
	return results_;
}

double BarrierOption::time() const
{
	return time_;
}

void BarrierOption::calculate_()
{
	clock_t begin = clock();		// begin time with threads

	computePrice_();
	computeDelta_();
	computeVega_();
	computeRho_();

	clock_t end = clock();		// end time with threads
	time_ = double(end - begin) / CLOCKS_PER_SEC;

	results_.resultSet.insert({ results_.PRICE, price_ });
	results_.resultSet.insert({ results_.DELTA, delta_ });
	results_.resultSet.insert({ results_.VEGA, vega_ });
	results_.resultSet.insert({ results_.RHO, rho_ });
}

// Private helper functions:
void BarrierOption::computePrice_()
{
	if (runParallel_)
	{
		computePriceAsync_();			
	}
	else
	{
		computePriceNoParallel_();
	}
}

void BarrierOption::computePriceNoParallel_()
{
	// ctor: EquityPriceGenerator(double initEquityPrice, unsigned numTimeSteps, 
	//                            double timeToMaturity, double drift, double volatility);
	EquityPriceGenerator epg(spot_, numTimeSteps_, tau_, riskFreeRate_, volatility_);
	vector<int> seeds;
	seeds.push_back(seed_);		// seed_ is actually our starting seed value

	for (unsigned i = 1; i < numScenarios_; ++i)
	{
		// This is a contrived way of setting a different seed for 
		// each scenario.  There are more robust ways to do this.
		seeds.push_back(seed_ + i);
	}

	vector<double> discountedPayoffs;

	for (auto& seed : seeds)
	{
		double terminalPrice = (epg(seed)).back();
		vector<double> priceVector = epg(seed);
		double payoff = 0.0;
		double existenceTime = 0.0;

		switch (BarrierType_)
		{
		case Barrier::DOWN_AND_OUT:
			// payoff = max(terminalPrice - strike_, 0.0);
			if (all_of(priceVector.begin(), priceVector.end(), 
				[this](double stp) {return stp> barrierLevel_;}))
			{
				payoff = 0.0;
			}
			else
			{
				auto knock = find_if(priceVector.begin(), priceVector.end(), 
					[this](double stp) {return stp < barrierLevel_;});
				payoff = strike_ - *knock;
				int pos = std::distance(priceVector.begin(), knock);
				existenceTime = settlement_*( (pos+1) /priceVector.size() );
			}
			break;
		case Barrier::UP_AND_OUT:
			// payoff = max(strike_ - terminalPrice, 0.0);
			if (all_of(priceVector.begin(), priceVector.end(), 
				[this](double stp) {return stp < barrierLevel_; }))
			{
				payoff = 0.0;
			}
			else
			{
				auto knock = find_if(priceVector.begin(), priceVector.end(), 
					[this](double stp) {return stp > barrierLevel_; });
				payoff = *knock - strike_;
				int pos = std::distance(priceVector.begin(), knock);
				existenceTime = settlement_ * ((pos + 1) / priceVector.size());
			}
			break;
		default:	// Put an assert here, as this should NEVER happen
			assert(false);
			break;
		}

		discountedPayoffs.push_back(discFactor_(0.0, existenceTime) * payoff);
	}

	price_ = quantity_ * (1.0 / discountedPayoffs.size()) * accumulate(discountedPayoffs.begin(), discountedPayoffs.end(), 0.0);

}

void BarrierOption::computePriceAsync_()
{
	EquityPriceGenerator epg(spot_, numTimeSteps_, tau_, riskFreeRate_, volatility_);

	vector<int> seeds;
	seeds.push_back(seed_);		// seed_ is actually our starting seed value

	for (unsigned i = 1; i < numScenarios_; ++i)	// We might, instead, wish to use
	{												// an arbitrary vector of seeds.
		seeds.push_back(seed_ + i);
	}

	// typedef vector<double> realVector;	// Old way
	using realVector = vector<double>;		// Modern C++ way
	vector<future<realVector> > futures;

	for (auto& seed : seeds)
	{
		futures.push_back(async(epg, seed));
	}

	vector<double> discountedPayoffs;

	for (auto& future : futures)
	{
		// double terminalPrice = future.get().back();
		vector<double> priceVector;
		priceVector = future.get();
		double payoff = 0.0;
		double existenceTime = 0.0;
		
		switch (BarrierType_)
		{
		case Barrier::DOWN_AND_OUT:
			// payoff = max(terminalPrice - strike_, 0.0);
			if (all_of(priceVector.begin(), priceVector.end(),
				[this](double stp) {return stp > barrierLevel_; }))
			{
				payoff = 0.0;
			}
			else
			{
				auto knock = find_if(priceVector.begin(), priceVector.end(), 
					[this](double stp) {return stp < barrierLevel_; });
				payoff = strike_ - *knock;
				int pos = std::distance(priceVector.begin(), knock);
				existenceTime = settlement_ * ((pos + 1) / priceVector.size());
			}
			break;
		case Barrier::UP_AND_OUT:
			// payoff = max(strike_ - terminalPrice, 0.0);
			if (all_of(priceVector.begin(), priceVector.end(), 
				[this](double stp) {return stp < barrierLevel_; }))
			{
				payoff = 0.0;
			}
			else
			{
				auto knock = find_if(priceVector.begin(), priceVector.end(), 
					[this](double stp) {return stp > barrierLevel_; });
				payoff = *knock - strike_;
				int pos = std::distance(priceVector.begin(), knock);
				existenceTime = settlement_ * ((pos + 1) / priceVector.size());
			}
			break;
		default:	// Should put assert here; this case should NEVER happen
			payoff = std::numeric_limits<double>::quiet_NaN();	// Returns NaN (Not a Number)
			break;
		}

		discountedPayoffs.push_back(discFactor_(0.0, existenceTime) * payoff);
	}

	price_ = quantity_ * (1.0 / discountedPayoffs.size()) * accumulate(discountedPayoffs.begin(), discountedPayoffs.end(), 0.0);
}

void BarrierOption::computeDelta_()
{
	double origSpot = spot_;
	double origPrice = price_;
	spot_ *= greekShift_;
	computePrice_();

	delta_ = (price_ - origPrice) / greekShift_;

	// reset state to original:
	spot_ = origSpot;
	price_ = origPrice;
}

void BarrierOption::computeVega_()
{
	double origVol = volatility_;
	double origPrice = price_;
	volatility_ *= greekShift_;
	computePrice_();

	vega_ = (price_ - origPrice) / greekShift_;

	// reset state to original:
	volatility_ = origVol;
	price_ = origPrice;
}

void BarrierOption::computeRho_()
{
	double origRfRate_ = riskFreeRate_;
	double origPrice = price_;
	riskFreeRate_ *= greekShift_;	// If we were using a term structure, we would need to 
									// shock the entire curve
	computePrice_();

	rho_ = (price_ - origPrice) / greekShift_;

	// reset state to original:
	riskFreeRate_ = origRfRate_;
	price_ = origPrice;
}

double BarrierOption::discFactor_(double yearFraction1, double yearFraction2)
{
	if (yearFraction1 <= yearFraction2)
	{
		return exp(-(yearFraction2 - yearFraction1)*riskFreeRate_);
	}
	else
	{
		// handle error:  For now, just return NaN (Not a Number).
		// Should put in an exception for production.
		return std::numeric_limits<double>::quiet_NaN();
	}
}
