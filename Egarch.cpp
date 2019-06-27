#include "Egarch.h"

using std::abs;
using std::log;
using std::pow;

Egarch::Egarch(double alphaZero, double alphaOne, double gamma, double beta, int seed)
	:alphaZero_(alphaZero),alphaOne_(alphaOne),gamma_(gamma),beta_(beta),seed_(seed)
{

}

double Egarch::operator()(double prevSigma, double norm) const
{
	return alphaZero_ + alphaOne_ * (abs(norm) + gamma_ * norm) + beta_ * log(prevSigma*prevSigma);
}
