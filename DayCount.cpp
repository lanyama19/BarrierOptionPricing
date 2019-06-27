#include "DayCount.h"
#include "Date.h"

// *** Class Act365 ***
double Act365::operator()(const Date& date1, const Date& date2) const
{
	return yearFraction(date1, date2);
}

double Act365::yearFraction(const Date& date1, const Date& date2) const
{
	return static_cast<double>(date2.serialDate() - date1.serialDate()) / 365.0;
}

// *** Class Act360 ***			(Homework Exercise)
double Act360::operator()(const Date & date1, const Date & date2) const
{
	return yearFraction(date1, date2);
}

double Act360::yearFraction(const Date & date1, const Date & date2) const
{
	return static_cast<double>(date2.serialDate() - date1.serialDate() )/365.0;
}

// *** Class Thirty360Eur ***	(Homework Exercise)

double Thirty360Eur::operator()(const Date & date1, const Date & date2) const
{
	return yearFraction(date1, date2);
}

double Thirty360Eur::yearFraction(const Date & date1, const Date & date2) const
{
	return static_cast<double>(dateDiff_(date1, date2))/360.0;
}

unsigned Thirty360Eur::dateDiff_(const Date & date1, const Date & date2) const
{
	unsigned diffDays;
	if (date1.month()==2 || date2.month()==2)
	{
		diffDays = 360 * (date2.year() - date1.year()) + 30*(date2.month() - date2.month())
			       -2 + date2.day() - date1.day();
	}
	else
	{
		diffDays = 360 * (date2.year() - date1.year()) + 30 * (date2.month() - date2.month())
			       + date2.day() - date1.day();
	}
	return diffDays;
};
