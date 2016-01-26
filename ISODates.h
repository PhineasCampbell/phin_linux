///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	This class is a set of routines to deal with dates for financial products.  The most basic thing it does is
//	generate payment schedules.  In general a financial instrument will pay a set of cashflows at various 
//  predetermined points in time e.g. semi annually on the 27 of Feburary and 27 August for the next 8 years.  
//  The issue is what to do if payments cannot be made on these dates. The class works by dealing with dates in two 
//  formats: the ISO format: YYYYMMDD and the number of days since 2000, converting between them as and when necessary.  
//	This will only deal with ISDA standard GBP dates
//
//	TODO:
//		Check unadjusted.  Does unadjusted really mean that the date will be unchanged even if it is a date on which
//		payments cannot be made.
//		Extend generating schedules to include IMM dates and last day of month
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cmath>

// In England there cannot be more than 4 days to the nearest good business day
const long MAX_DAYS_TO_GOOD_DAY = 4;
// The number of elements in the Bank Holiday array
const long BANK_HOLIDAY_COUNT = 80;
// Cummulative number of days before the end of the month for a non leap year
const long NON_LEAP_YEAR_MONTH_DAYS[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
// Cummulative number of days before the end of the month for a leap year
const long LEAP_YEAR_MONTH_DAYS[12] = {0,31,60,91,121,152,182,213,244,274,305,335};
// The number of days in the month for a leap year
const long NON_LEAP_YEAR_MONTH_LENGTH[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
// The number of days in the month for a non leap year
const long LEAP_YEAR_MONTH_LENGTH[13] = {0,31,29,31,30,31,30,31,31,30,31,30,31};
// The GBP non business days in years since 2000 format
const long GBP_HOLIDAYS[BANK_HOLIDAY_COUNT] = {5479,5571,5574,5599,5629,5721,5837,5840,5844,5928,5931,5965,5995,6087,
	                                           6204,6205,6210,6313,6316,6330,6360,6452,6568,6569,6575,6663,6666,6695,
											   6725,6817,6933,6934,6940,7048,7051,7060,7090,7182,7298,7299,7305,7405,
											   7408,7426,7456,7548,7664,7667,7671,7762,7765,7791,7821,7913,8031,8032,
											   8038,8140,8143,8156,8186,8278,8395,8396,8403,8497,8500,8521,8551,8643,
											   8759,8760,8766,8854,8857,8887,8917,9009,9125,9126};
// The class will fail if you ask for a date more than 100 years in advance
const long MAX_YEARS = 100; 
// The number of days in a non leap year year, added to avoid magic numbers
const double DAYS_PER_YEAR = 365.0;
// The number of days in a leap year year.
const double DAYS_PER_LEAP_YEAR = 366.0;

// One of D(ay), W(eek), M(onth) or Y(ear)
enum DateUnit{D,W,M,Y};

// One of P(receeding), F(ollowing), MF (M)odified (F)ollowing or N(one)
enum AdjustmentTypes{P,F,MF,N};


class ISODate
{
public:
	ISODate();
	ISODate(long dayOfMonth, long Month, long Year);
	ISODate(long date);
	// Empty destructor
	~ISODate(){}
	void SetDateAdjustment(AdjustmentTypes adjustmentType);

	// Return the number of days since 2000 from the stored day, month, year
	long DaysSince2000();
	// Return the number of days since 2000 from the input day, month, year
	long DaysSince2000(long dayOfMonth, long Month, long Year);
	// Set the date from and ISO date
	void SetDate(long date);
	// Set the date in dd,mm,yyyy format
	void SetDate(long dayOfMonth, long Month, long Year);
	// Increment the date by number of DateUnits and return the date in the form of ISODate or days since 2000
	long DateIncrement(DateUnit dp, long number, bool returnAsDaysSince2000 = true);
	// Return true if the year is a leap year
	bool IsLeapYear(long Year);
	// Return true if daysSince2000 is a good business day
	bool IsGoodBusinessDay(long daysSince2000);
	// The number of days from daysSince2000 to a good business day 
	long DaysToGoodBusinessDay(long daysSince2000, bool moveForward = true);
	// Given the date in the form of years since 2000 return the ISO form: YYYYMMDD
	long DateSince2000(long daysSince2000);
	// Return the next good business day
	long NextGoodBusinessDay(long dayOfMonth, long Month, long Year, AdjustmentTypes = MF);
	// Return true if daysSince2000 is a bank holiday
	bool IsBankHoliday(long daysSince2000);
	// Convert ISODate to Days Since 2000
	long ConvertToDaysSince2000(long date);
	// Given the day of month, the month number and the year number return the ISO date form
	long ConvertToISO(long dayOfMonth, long Month, long Year)  // note inlined
	{
		return dayOfMonth + Month * 100 + Year*10000;
	}
	// Return the date in ISO format
	long ReturnAsISO()
	{
		return _dayOfMonth + _monthNumber * 100 + _year*10000;
	}
	// Given a start date and end date return the number of years between the two dates assuming a 365 day year
	double YearFrac(long ISO_Start, long ISO_End);
private:
	// Day of the month: 1 to 31
	long _dayOfMonth;
	// Month of the year: 1 to 12
	long _monthNumber;
	// Year in YYYY form from 2000
	long _year;
	// The date adjustment type Modified Following, Following Preceding and Unadjusted
	AdjustmentTypes _dateAdjustment;
};