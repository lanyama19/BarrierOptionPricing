#ifndef EGARCH_H	
#define EGARCH_H

#include <cmath>
#include <random>

using std::mt19937_64;
using std::normal_distribution;
using std::exp;
using std::vector;

class Egarch
{
public:
	Egarch(double alphaZero, double alphaOne, double gamma, double beta, int seed);
	double operator() (double prevSigma,double norm) const;
private:
	double alphaZero_;
	double alphaOne_;
	double gamma_;
	double beta_;
	int seed_;
};

#endif
