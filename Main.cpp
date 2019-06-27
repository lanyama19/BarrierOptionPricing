#include "EquityPriceGenerator.h"
#include "BarrierOption.h"
#include "ResultSet.h"
#include "Date.h"
#include <iostream>
#include <algorithm>
#include <boost/circular_buffer.hpp>
#include "Egarch.h"

using std::vector;
using std::cout;
using std::endl;
using std::for_each;
using std::exp;

void mcBarrCall();
void simVolatilties(double alphaZero, double alphaOne, double beta,double gamma, int seed, double initSigma, int bufferSize);

int main() 
{
	mcBarrCall();
	simVolatilties(-0.0883, 0.1123, 0.9855, -0.0925, 520, 0.25, 100);
	return 0;
}

void mcBarrCall()
{
	double barrierLevel = 103.0;
	double strike = 102.0;
	double spot = 100.0;
	double riskFreeRate = 0.025;
	double volatility = 0.06;
	double quantity = 7000.00;	// 1.0;
	Barrier up_out = Barrier::UP_AND_OUT;
	Date valueDate(2015, 10, 1);
	Date expiryDate(2017, 9, 30);
	Date settlementDate(expiryDate.addDays(1));		// expiryDate + 1
	unsigned numTimeSteps = 720;
	unsigned numScenarios = 10000;
	int seed = -106;
	double greekShift = 0.01;

	Act365 act365;

	BarrierOption upOutBarrier(barrierLevel,strike, spot, riskFreeRate, volatility,quantity, 
		up_out, valueDate, expiryDate, settlementDate, numTimeSteps, numScenarios, true,
		seed, greekShift, act365);
	OptionResults res = upOutBarrier();
	res.print();
	cout << "Runtime (IS RUN in parallel): " << upOutBarrier.time() << endl << endl;
};

void simVolatilties(double alphaZero, double alphaOne, double beta, 
					double gamma, int seed, double initSigma, int bufferSize)
{
	cout << "Simulate Volatilities using EGARCH model: " << endl;
	try
	{
		boost::circular_buffer<double> simVol(bufferSize);
		Egarch egarch(alphaZero, alphaOne, gamma, beta, seed);
		simVol.push_back(initSigma);
		double prevSigma = initSigma;
		mt19937_64 mtre(seed);
		normal_distribution<> nd;
		for (int i = 1; i <= bufferSize; ++i)
		{
			prevSigma = sqrt(exp(egarch(prevSigma, nd(mtre))));
			simVol.push_back(prevSigma);
		}
		auto printDouble = [](const double& vv)
		{
			cout << vv << " ";
		};
		cout << "First three elements are: ";
		for_each(simVol.begin(), simVol.begin() + 3, printDouble);
		cout << endl;
		cout << "Last three elements are: ";
		for_each(simVol.end()-3, simVol.end(), printDouble);
		cout << endl;
	}
	catch (const std::length_error& e)
	{
		cout << "Circular buffer error(): " << e.what() << endl;
	}
	
};