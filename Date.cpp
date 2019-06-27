#include "Date.h"
#include <stdexcept>	// Need this for out_of_range and invalid argument exceptions
// See http://www.cplusplus.com/reference/stdexcept/out_of_range/
using std::out_of_range;
using std::invalid_argument;

/*
	This implementation borrows from the following sources:
	a) http://www.codeproject.com/Articles/2750/Excel-serial-date-to-Day-Month-Year-and-vise-versa
	b) The QuantLib Date class
*/

Date::Date() {}

Date::Date(int serialDate) :serialDate_(serialDate)
{
	if (!serialToDate_())
	{
		out_of_range e("Date: Serial Date is out of range in constructor.");
		throw e;
	}
}

Date::Date(int year, int month, int day) :year_(year), month_(month), day_(day)
{
	if (!dateToSerial_())
	{
		out_of_range e("Date: Calendar Date is out of range in constructor.");
		throw e;
	}
}

Date& Date::addYears(int years)
{
	if (endOfMonth())
	{
		year_ += years;
		day_ = daysInMonth();
	}
	else
	{
		year_ += years;
	}

	bool torf = dateToSerial_();

	if (!dateToSerial_())
	{
		out_of_range e("Date::addYears(.): resulting date out of range.");
		throw e;
	}

	return *this;

}

Date& Date::addMonths(int months)
{
	if (endOfMonth())
	{
		month_ += months;
		while (month_ > 12) {
			month_ -= 12;
			year_ += 1;
		}
		while (month_ < 1) {
			month_ += 12;
			year_ -= 1;
		}
		day_ = daysInMonth();

	}
	else
	{
		month_ += months;
		while (month_ > 12)
		{
			month_ -= 12;
			year_ += 1;
		}
		while (month_ < 1)
		{
			month_ += 12;
			year_ -= 1;
		}

	}

	if (!dateToSerial_())
	{
		out_of_range e("Date::addMonths(.): resulting date out of range.");
		throw e;
	}

	return *this;
}

Date& Date::addDays(int days)
{
	serialDate_ += days;

	if (!serialToDate_())
	{
		out_of_range e("Date::addDays(.): resulting date out of range.");
		throw e;
	}

	return *this;
}

bool Date::endOfMonth() const
{
	if (day_ == daysInMonth())
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Date::daysInMonth() const
{
	static const int monthLength[] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	static const int monthLeapLength[] = {
		31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	return (leapYear() ? monthLeapLength[month_ - 1] : monthLength[month_ - 1]);
}

bool Date::leapYear() const
{
	static const bool yearIsLeap[] = {
		// 1900 is leap in agreement with Excel's bug
		// 1900 is out of valid date range anyway
		// 1900-1909
		true,false,false,false, true,false,false,false, true,false,
		// 1910-1919
		false,false, true,false,false,false, true,false,false,false,
		// 1920-1929
		true,false,false,false, true,false,false,false, true,false,
		// 1930-1939
		false,false, true,false,false,false, true,false,false,false,
		// 1940-1949
		true,false,false,false, true,false,false,false, true,false,
		// 1950-1959
		false,false, true,false,false,false, true,false,false,false,
		// 1960-1969
		true,false,false,false, true,false,false,false, true,false,
		// 1970-1979
		false,false, true,false,false,false, true,false,false,false,
		// 1980-1989
		true,false,false,false, true,false,false,false, true,false,
		// 1990-1999
		false,false, true,false,false,false, true,false,false,false,
		// 2000-2009
		true,false,false,false, true,false,false,false, true,false,
		// 2010-2019
		false,false, true,false,false,false, true,false,false,false,
		// 2020-2029
		true,false,false,false, true,false,false,false, true,false,
		// 2030-2039
		false,false, true,false,false,false, true,false,false,false,
		// 2040-2049
		true,false,false,false, true,false,false,false, true,false,
		// 2050-2059
		false,false, true,false,false,false, true,false,false,false,
		// 2060-2069
		true,false,false,false, true,false,false,false, true,false,
		// 2070-2079
		false,false, true,false,false,false, true,false,false,false,
		// 2080-2089
		true,false,false,false, true,false,false,false, true,false,
		// 2090-2099
		false,false, true,false,false,false, true,false,false,false,
		// 2100-2109
		false,false,false,false, true,false,false,false, true,false,
		// 2110-2119
		false,false, true,false,false,false, true,false,false,false,
		// 2120-2129
		true,false,false,false, true,false,false,false, true,false,
		// 2130-2139
		false,false, true,false,false,false, true,false,false,false,
		// 2140-2149
		true,false,false,false, true,false,false,false, true,false,
		// 2150-2159
		false,false, true,false,false,false, true,false,false,false,
		// 2160-2169
		true,false,false,false, true,false,false,false, true,false,
		// 2170-2179
		false,false, true,false,false,false, true,false,false,false,
		// 2180-2189
		true,false,false,false, true,false,false,false, true,false,
		// 2190-2199
		false,false, true,false,false,false, true,false,false,false,
		// 2200
		false
	};

	return yearIsLeap[year_ - 1900];
}

int Date::year() const
{
	return year_;
}

int Date::month() const
{
	return month_;
}

int Date::day() const
{
	return day_;
}

int Date::serialDate() const
{
	return serialDate_;
}

void Date::setYear(int year)
{
	year_ = year;
	if (!dateToSerial_())
	{
		out_of_range e("Date::setYear(.): resulting date out of range.");
		throw e;
	}

}

void Date::setMonth(int month)
{
	month_ = month;
	if (!dateToSerial_())
	{
		out_of_range e("Date::setMonth(.): resulting date out of range.");
		throw e;
	}
}

void Date::setDay(int day)
{
	day_ = day;
	if (!dateToSerial_())
	{
		out_of_range e("Date::setDay(.): resulting date out of range.");
		throw e;
	}
}

void Date::setSerialDate(int serialDate)
{
	serialDate_ = serialDate;
	if (!serialToDate_())
	{
		out_of_range e("Date::setSerialDate(.): resulting date out of range.");
		throw e;
	}
}

int Date::getYear() const
{
	return year();
}

int Date::getMonth() const
{
	return month();
}

int Date::getDay() const
{
	return day();
}

int Date::operator()() const
{
	return serialDate();
}

int Date::operator - (const Date& rhs) const
{
	return serialDate_ - rhs.serialDate_;
}

Date& Date::operator += (int days)
{
	serialDate_ += days;
	if (serialDate_ <= maxSerial_)
	{
		serialToDate_();
		return *this;
	}
	else
	{
		out_of_range e("Date: Serial Date is out of range in operator (+=).");
		throw e;
	}
}

Date& Date::operator -= (int days)
{
	serialDate_ += days;
	if (serialDate_ >= minSerial_)
	{
		serialToDate_();
		return *this;
	}
	else
	{
		out_of_range e("Date: Serial Date is out of range in operator (-=).");
		throw e;
	}
}

Date& Date::operator ++ ()
{
	++serialDate_;
	if (serialDate_ <= maxSerial_)
	{
		serialToDate_();
		return *this;
	}
	else
	{
		out_of_range e("Date: Serial Date is out of range in pre-increment operator (++).");
		throw e;
	}
}

Date& Date::operator -- ()
{
	--serialDate_;
	if (serialDate_ >= minSerial_)
	{
		serialToDate_();
		return *this;
	}
	else
	{
		out_of_range e("Date: Serial Date is out of range in pre-decrement operator (--).");
		throw e;
	}
}

Date Date::operator ++ (int notused)
{
	Date d(*this);
	++(*this);

	if (d.serialDate() <= maxSerial_)
	{
		return d;
	}
	else
	{
		out_of_range e("Date: Serial Date is out of range in post-increment operator (++).");
		throw e;
	}

}

Date Date::operator -- (int notused)
{
	Date d(*this);
	--(*this);

	if (d.serialDate() >= minSerial_)
	{
		return d;
	}
	else
	{
		out_of_range e("Date: Serial Date is out of range in post-decrement operator (--).");
		throw e;
	}

}

bool Date::operator == (const Date& rhs) const
{
	return (serialDate_ == rhs.serialDate_);
}
bool Date::operator < (const Date& rhs) const
{
	return (serialDate_ < rhs.serialDate_);
}
bool Date::operator > (const Date& rhs) const
{
	return (serialDate_ > rhs.serialDate_);
}
bool Date::operator <= (const Date& rhs) const
{
	return ((serialDate_ < rhs.serialDate_) || (serialDate_ == rhs.serialDate_));
}
bool Date::operator >= (const Date& rhs) const
{
	return ((serialDate_ > rhs.serialDate_) || (serialDate_ == rhs.serialDate_));
}

bool Date::serialToDate_()
{
	bool torf;

	if ((serialDate_ < minSerial_) || (serialDate_ > maxSerial_))
	{
		torf = false;
	}

	else if (serialDate_ == 60)
	{
		year_ = 1900;
		month_ = 2;
		day_ = 29;

		torf = true;
	}
	else if (serialDate_ < 60)
	{
		// Because of the 1900.02.29 bug, any serial date 
		// under 60 is one off... Compensate.
		++serialDate_;

		torf = true;
	}
	else
	{
		// Modified Julian to YMD calculation with an addition of 2415019
		int m = serialDate_ + 68569 + 2415019;
		int n = (4 * m) / 146097;
		m -= (146097 * n + 3) / 4;

		int i = (4000 * (m + 1)) / 1461001;
		m -= (1461 * i) / 4 - 31;

		int j = (80 * m) / 2447;
		day_ = m - (2447 * j) / 80;
		m = j / 11;
		month_ = j + 2 - (12 * m);
		year_ = 100 * (n - 49) + i + m;

		torf = true;
	}

	return torf;
}

bool Date::dateToSerial_()
{
	bool torf;

	if ((year_ < minYear_) || (year_ > maxYear_))
	{
		torf = false;
	}

	// Excel/Lotus 123 have a bug with 1900.02.29. 1900 is not a
	// leap year, but Excel/Lotus 123 thinks it is...
	else if (day_ == 29 && month_ == 2 && year_ == 1900)
	{
		serialDate_ = 60;
		torf = true;
	}
	else if (month_ < 1 || month_ > 12)
	{
		torf = false;
	}
	else if (day_ < 1 || day_ > daysInMonth())
	{
		torf = false;
	}
	else
	{
		// YMD to Modified Julian: calculate with an extra subtraction of 2415019.
		int a = (14 - month_) / 12;
		int m = month_ + 12 * a - 3;
		int y = year_ + 4800 - a;
		serialDate_ = day_ + int((153 * m + 2) / 5) + 365 * y + int(y / 4) - int(y / 100) + int(y / 400) - 32045 - 2415019;

		torf = true;
	}

	if (serialDate_ < 60)
	{
		// Because of the 29-02-1900 bug, any serial date 
		// under 60 is one off... Compensate.
		--serialDate_;
	}

	return torf;
}

// This is a 'friend' of the Date class
std::ostream& operator << (std::ostream& os, const Date& rhs)
{
	os /* << "yyyy.mm.dd = " */ << rhs.year_ << "." << rhs.month_ << "." << rhs.day_ << "; Excel serial format = " << rhs.serialDate_;
	return os;
}
