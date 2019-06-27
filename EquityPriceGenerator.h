#ifndef EQUITY_PRICE_GENERATOR_H
#define EQUITY_PRICE_GENERATOR_H

#include <vector>

class EquityPriceGenerator
{
public:
	// A more robust approach would be to add in a stub period at beginning
	EquityPriceGenerator(double initEquityPrice, unsigned numTimeSteps, double timeToMaturity, double drift, double volatility);

	// We could also have another ctor that takes in a TermStructure object in place of a constant drift or risk free rate,
	// as well as a time path determined by a schedule based on dates and a daycount rule; viz,
	// EquityPriceGenerator(double initEquityPrice, const RealSchedule& realSchedule, const TermStructure& ts, double volatility);

	std::vector<double> operator()(int seed) const;

private:
	double yearFraction_;
	const double initEquityPrice_;
	const int numTimeSteps_;
	const double timeToMaturity_;
	const double drift_;
	const double volatility_;
};

#endif

