#ifndef RESULT_SET_H
#define RESULT_SET_H

#include <map>
#include <iostream>

enum class OptionType
{
	CALL,
	PUT
};

enum class Barrier 
{
	UP_AND_OUT,
	DOWN_AND_OUT
};

struct OptionResults
{
	enum Value	// Keep as regular (integer) enum so that we can use as the key value in an std::map
	{
		// Since it is encapsulated within the struct, it does not pollute the global namespace.
		// Result set will be the price of the option and the 1st order risk values (ie, omit gamma)
		PRICE, // The value of the option as a result of the pricing model
		DELTA, // Option delta
		VEGA,  // Option vega
		RHO    // Option rho
	};

	std::map<Value, double> resultSet;

	void print()
	{
		std::cout << std::endl;
		std::cout << "Option Price = " << resultSet.at(PRICE) << std::endl;
		std::cout << "Option Delta = " << resultSet.at(DELTA) << std::endl;
		std::cout << "Option Vega = " << resultSet.at(VEGA) << std::endl;
		std::cout << "Option Rho = " << resultSet.at(RHO) << std::endl << std::endl;
	};
};

struct BondResults
{
	enum Value	// Keep as regular (integer) enum so that we can use as the key value in an std::map
	{
		// Since it is encapsulated within the struct, it does not pollute the global namespace.
		PRICE,		// The value of the bond
		YTM,		// Yield to Maturity
		DURATION	// Bond duration (requires YTM value)
	};

	std::map<Value, double> resultSet;

	void print()
	{
		std::cout << std::endl;
		std::cout << "Bond Price = " << resultSet.at(PRICE) << std::endl;
		std::cout << "Bond Duration = " << resultSet.at(DURATION) << std::endl;
		std::cout << "Yield to Maturity = " << resultSet.at(YTM) << std::endl << std::endl;
	};
};

#endif
