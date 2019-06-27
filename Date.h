#ifndef DATE_H
#define DATE_H
#include <iostream>


class Date
{
public:
	Date();
	Date(int serialDate);								
	// throws out_of_range
	Date(int year, int month, int day);		// throws out_of_range

	// Return *this in case we want to do something like
	// Date d2 = d1.addYears(5);
	Date& addYears(int years);
	Date& addMonths(int months);
	Date& addDays(int days);

	int daysInMonth() const;
	bool endOfMonth() const;
	bool leapYear() const;

	int year() const;
	int month() const;
	int day() const;
	int serialDate() const;

	void setYear(int year);
	void setMonth(int month);
	void setDay(int day);
	void setSerialDate(int serialDate);

	int getYear() const;
	int getMonth() const;
	int getDay() const;
	int operator()() const;		// Alternative accessor for serialDate_
	int operator - (const Date& rhs) const;

	Date& operator += (int days);
	Date& operator -= (int days);

	Date& operator ++ ();
	Date& operator -- ();

	Date operator ++ (int notused);
	Date operator -- (int notused);

	bool operator == (const Date& rhs) const;
	bool operator < (const Date& rhs) const;
	bool operator > (const Date& rhs) const;
	bool operator <= (const Date& rhs) const;
	bool operator >= (const Date& rhs) const;

	// friend operator so that we can output date details with cout
	friend std::ostream& operator << (std::ostream& os, const Date& rhs);

private:
	bool serialToDate_();
	bool dateToSerial_();

	int serialDate_;
	int year_;
	int month_;
	int day_;

	// This can only be done in the header for integer types (prior to C++11)
	static const int minSerial_ = 1;		// 1900.01.01: Matches Excel
	static const int maxSerial_ = 109574;	// 2199.12.31: Matches Excel
	static const int minYear_ = 1900;
	static const int maxYear_ = 2199;
};




#endif

#pragma once
