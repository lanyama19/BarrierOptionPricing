#ifndef BARRIER_OPTION
#define BARRIER_OPTION

#include "Date.h"
#include "DayCount.h"
#include "ResultSet.h"
#include "EquityPriceGenerator.h"


class BarrierOption
{
	// Proposed defaults: bool runParallel = true, int seed = 0, double greekShift = 0.01
public:
	BarrierOption(double barrierLevel,double strike, double spot, double riskFreeRate, double volatility,
		double quantity, Barrier BarrierType, const Date& valueDate, const Date& expiryDate,
		const Date& settlementDate, unsigned numTimeSteps, unsigned numScenarios, bool runParallel,
		int seed, double greekShift, const Act365& dc);

	OptionResults operator()() const;
	double time() const;		// Time required to run calcutions (for comparison using concurrency)

private:
	void calculate_();			// Compute price and risk values (called from ctor)

	// Indicates whether to run pricing scenarios in parallel
	bool runParallel_;			// default = true

	// Private helper functions:
	void computePrice_();
	void computeDelta_();
	//	void computeGamma_();
	void computeVega_();
	void computeRho_();

	// Compare results:  non-parallel vs in-parallel with async and futures
	void computePriceNoParallel_();
	void computePriceAsync_();

	// Compute discount factor P(t1, t2)
	double discFactor_(double yearFactor1, double yearFactor2);

	// Inputs to model:
	Barrier BarrierType_;	// porc_: put or call
	double barrierLevel_;
	double spot_;
	double strike_;
	double riskFreeRate_;
	double volatility_;
	double quantity_;
	unsigned numTimeSteps_;
	double yearFraction_;
	double greekShift_;		// Percentage shift to use for risk value approximations (default = 1%)
	double tau_;			// Daycount adjusted time to expiration (as year fraction)
	double settlement_;		// Daycount adjusted time to settlement (as year fraction)
	unsigned numScenarios_;
	int seed_;

	// Calculated values stored in these private members:
	double price_;
	double delta_;
	//	double gamma_;
	double vega_;
	double rho_;

	// Runtime comparison using concurrency
	double time_;

	// Result set of option values:
	OptionResults results_;
};


#endif // !BARRIER_OPTION

