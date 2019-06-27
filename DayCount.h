#ifndef DAY_COUNT_H
#define DAY_COUNT_H

#include "Date.h"


class DayCount
{
public:
	// The operator should just call the member function yearFraction(.)
	virtual double operator() (const Date& date1, const Date& date2) const = 0;
	virtual double yearFraction(const Date& date1, const Date& date2) const = 0;
	virtual ~DayCount() = default;
};


class Act365 : public DayCount
{
public:
	virtual double operator() (const Date& date1, const Date& date2) const override;
	virtual double yearFraction(const Date& date1, const Date& date2) const override;
};


class Act360 : public DayCount
{
public:
	virtual double operator() (const Date& date1, const Date& date2) const override;
	virtual double yearFraction(const Date& date1, const Date& date2) const override;
};


class Thirty360Eur : public DayCount	// see https://sqlsunday.com/2014/08/17/30-360-day-count-convention/
										// The Excel equivalent of this is YEARFRAC(fromDate; toDate; 4)
{
public:
	virtual double operator() (const Date& date1, const Date& date2) const override;
	virtual double yearFraction(const Date& date1, const Date& date2) const override;

private:
	unsigned dateDiff_(const Date& date1, const Date& date2) const;
};



#endif
