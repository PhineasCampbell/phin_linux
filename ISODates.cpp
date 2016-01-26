#include "ISODates.h"


// Minimum Constructor
ISODate::ISODate()
{
	_year = 0;
	_monthNumber = 0;
	_dayOfMonth = 0;
	_dateAdjustment = MF;
	return;
}


// Constructor using day, month, year
ISODate::ISODate(long dayOfMonth, long Month, long Year)
{
	_dayOfMonth = dayOfMonth;
	_monthNumber = Month;
	_year = Year;
	_dateAdjustment = MF;
	return;
}


// Constructor using the ISO 8601 YYYYMMDD date format
ISODate::ISODate(long Date)
{
	SetDate(Date);
	_dateAdjustment = MF;
	return;
}


// Given a date in the ISO 8601 YYYYMMDD date format set the date members of the class
void ISODate::SetDate(long Date)
{
	_year = Date/10000;
	long monthAndDay = (Date - _year*10000);
	_monthNumber = monthAndDay/100;
	_dayOfMonth = monthAndDay - _monthNumber*100;

	return;
}


// Set the Day, Month and Year of the class
void ISODate::SetDate(long dayOfMonth, long Month, long Year)
{
	_dayOfMonth = dayOfMonth;
	_monthNumber = Month;
	_year = Year;
	return;
}


// Set the date adjustment: mf, adj, unadj, f
void ISODate::SetDateAdjustment(AdjustmentTypes adjustmentType)
{
	_dateAdjustment = adjustmentType;
}


// Return True if Year is a leap year
bool ISODate::IsLeapYear(long Year)
{
	if(Year % 4 == 0)
		return true;
	else
		return false;
}


// Given the Day of Month, Month Number and Year return the number of days since 01 Jan 2000
long ISODate::DaysSince2000(long dayOfMonth, long Month, long Year)
{
	// Get the number of days till the start of the month
	long monthDays = 0;
	if(IsLeapYear(Year))
		monthDays = LEAP_YEAR_MONTH_DAYS[Month-1];
	else 
		monthDays = NON_LEAP_YEAR_MONTH_DAYS[Month-1];

	// Get the number of days till the start of the year
	long yearDays = 0;
	long yearCount = Year;
	// Loop down through the years accumulating the days
	while(yearCount > 2000)
	{
		if(IsLeapYear(yearCount-1))
			yearDays += (long)DAYS_PER_LEAP_YEAR;
		else
			yearDays += (long)DAYS_PER_YEAR;
		yearCount--;
	}
	return yearDays + monthDays + dayOfMonth-1;
}


// Return the the days since 01 Jan 2000
long ISODate::DaysSince2000()
{
	return DaysSince2000(_dayOfMonth,_monthNumber,_year);
}


// Increment the date by <number> of units and return it in either the ISO or days since 2000 format
// The default is days since 2000 
long ISODate::DateIncrement(DateUnit dp, long number, bool returnAsDaysSince2000)
{
	long dateToBeReturned = 0;

	// We populate these upfront as they will always be used
	long day = _dayOfMonth;
	long month = _monthNumber;
	long year = _year;
	long yearAdjustment = 0;
	long newMonth = 0;
	long newYear = 0;

	// Adjust by days or weeks
	if(dp == D || dp == W)
	{
		// A week is just 7 days
		if(dp == W)
			number *= 7;
		day += number;
		dateToBeReturned = NextGoodBusinessDay(day,month,year);
	}
	// Adjust by months or years
	else if(dp == M || dp == Y)
	{
		// A year is just 12 months
		if(dp == Y)
			number *= 12;

		month += number;
		yearAdjustment = (long)std::floor((double)(month-1)/12);
		newYear = yearAdjustment;
		newMonth = month - 12*yearAdjustment;
		year += newYear;

		dateToBeReturned = NextGoodBusinessDay(day,newMonth,year);
	}
	else
	{
		// Empty else
	}

	if(returnAsDaysSince2000 == false)
		dateToBeReturned = DateSince2000(dateToBeReturned);
	return dateToBeReturned;
}


// Given a date in the days since 2000 format find the next good business day by moving either forward or backward
// forward is default
long ISODate::DaysToGoodBusinessDay(long daysSince2000, bool moveForward)
{
	long days = 0;
	
	long dayOfWeek = daysSince2000 % 7;
	bool isBankHoliday = IsBankHoliday(daysSince2000);

	// We have an early exit point if the day is not a saturday, sunday or bank holiday then we have no days to add
	if(dayOfWeek != 0 && dayOfWeek != 1 && isBankHoliday == false)
	{
		return days;
	}

	// We are moving backward to find the good business day
	if(moveForward == false)
	{
		for(days = -1; days>-1*MAX_DAYS_TO_GOOD_DAY; days--)
		{
			dayOfWeek = (daysSince2000+days) % 7;
			isBankHoliday = IsBankHoliday(daysSince2000+days);
			if(dayOfWeek != 0 && dayOfWeek != 1 && isBankHoliday == false)
			{
				break;
			}
		}
	}
	// Otherwise move forward
	else
	{
		for(days = 1; days<MAX_DAYS_TO_GOOD_DAY; days++)
		{
			dayOfWeek = (daysSince2000+days) % 7;
			isBankHoliday = IsBankHoliday(daysSince2000+days);
			if(dayOfWeek != 0 && dayOfWeek != 1 && isBankHoliday == false)
			{
				break;
			}
		}
	}
	return days;
}


// Return True if the date corresponding to daysSince2000 is a good business day
bool ISODate::IsGoodBusinessDay(long daysSince2000)
{
	bool retVal = false;
	// Get the day of the week
	long dayOfWeek = daysSince2000 % 7;

	bool isBankHoliday = IsBankHoliday(daysSince2000);

	// If it is a weekday and not a bank holiday then it is a good day
	if(dayOfWeek > 1  && isBankHoliday == false)
	{
		retVal = true;
	}

	return retVal;
}


// Return true if daysSince2000 is a bank holiday.  The code is lifted from the function locate in section 3.4 in Numerical
// Recipes in C
bool ISODate::IsBankHoliday(long daysSince2000)
{
	long ju, jm, jl;  // Upper mid and lower
	long ascnd;
	bool found = false;

	jl = 0;
	ju = BANK_HOLIDAY_COUNT;

	ascnd = (GBP_HOLIDAYS[BANK_HOLIDAY_COUNT-1]>= GBP_HOLIDAYS[0]);

	while(ju-jl>1)
	{
		jm = (ju+jl) >> 1;
		if(daysSince2000 >= GBP_HOLIDAYS[jm] == ascnd)
		{
			jl = jm;
		}
		else
		{
			ju = jm;
		}
	}

	if(daysSince2000 == GBP_HOLIDAYS[0])
	{
		found = true;
	}
	else if(daysSince2000 == GBP_HOLIDAYS[BANK_HOLIDAY_COUNT-1])
	{
		found = true;
	}
	else if(daysSince2000 == GBP_HOLIDAYS[jl])
	{
		found = true;
	}
	return found;
}


// Given the date in the form of days since 2000 return the date in the ISO format
long ISODate::DateSince2000(long daysSince2000)
{
	long yearCount;
	long yearDayCount = 0;

	// True if the year is a leap year
	bool isLeapYear = false;

	// Extract the year part  
	for(yearCount=0; yearCount<MAX_YEARS; yearCount++)
	{
		// If this year is a leap year
		isLeapYear = IsLeapYear(yearCount+2000);
		// If this year is a leap year
		if(isLeapYear)
		{
			if(yearDayCount + DAYS_PER_LEAP_YEAR > daysSince2000)
				break;
			else
				yearDayCount += (long)DAYS_PER_LEAP_YEAR;
		}
		else
		{
			if(yearDayCount + DAYS_PER_YEAR > daysSince2000)
				break;
			else
				yearDayCount += (long)DAYS_PER_YEAR;
		}
	}
	
	// Extract the month part
	// The number of days since the start of this year
	long monthDays = daysSince2000 - yearDayCount+1; 
	// The number of months we are looking at
	long monthCount = 0;
	// The day of month
	long monthDayCount = 1;
	// Loop up through the months till we find the first day greater than the day part
	for(monthCount = 1; monthCount < 12; monthCount++)
	{
		// If the year we are dealing with is a leap year
		if(isLeapYear)
		{
			// If we are going to cross the boundary
			if(monthDayCount+LEAP_YEAR_MONTH_LENGTH[monthCount] > monthDays)
				break;
			else
				monthDayCount+=LEAP_YEAR_MONTH_LENGTH[monthCount];
		}
		// This isn't a leap year
		else
		{
			// If we are going to cross the boundary
			if(monthDayCount+NON_LEAP_YEAR_MONTH_LENGTH[monthCount] > monthDays)
				break;
			else
				monthDayCount+=NON_LEAP_YEAR_MONTH_LENGTH[monthCount];
		}
	}
	
	// Now extract the day part
	long dayOfMonth = 0;
	if(isLeapYear)
	{
		dayOfMonth = monthDays - LEAP_YEAR_MONTH_DAYS[monthCount-1];
	}
	else
	{
		dayOfMonth = monthDays - NON_LEAP_YEAR_MONTH_DAYS[monthCount-1];
	}
	return ConvertToISO(dayOfMonth,monthCount,yearCount+2000);
}


// Given a date in the format dd, mm, yyyy return the next good business day adjusted according to adj
long ISODate::NextGoodBusinessDay(long dayOfMonth, long Month, long Year, AdjustmentTypes adj)
{
	long daysSince2000 = DaysSince2000(dayOfMonth,Month,Year);
	long daysForward = 0;

	// We have an early exit point if the day is not a Saturday, Sunday or bank holiday then we have no days to add
	if(IsGoodBusinessDay(daysSince2000))
	{
		return daysSince2000;
	}

	// If the date is unadjusted we don't need to check adjustment
	if(adj == N)
	{
		return daysSince2000;
	}

	// If the adjustment type is following or modidied following
	if(adj == MF || adj == F)
	{
		for(daysForward = 1; daysForward <= MAX_DAYS_TO_GOOD_DAY; daysForward++)
		{
			daysSince2000 += 1;
			if(IsGoodBusinessDay(daysSince2000))
				break;
		}
		// If is following we don't need to do any adjustment
		if(adj == F)
		{
			return daysSince2000;
		}
		else if(adj == MF)
		{
			// Do we break the month barrier
			if(IsLeapYear(Year))
			{
				if(dayOfMonth+daysForward > LEAP_YEAR_MONTH_LENGTH[Month])
				{
					for(daysForward = MAX_DAYS_TO_GOOD_DAY; daysForward > 0; daysForward--)
					{
						daysSince2000 += 1;
						if(IsGoodBusinessDay(daysSince2000))
							break;
					}
				}
			}
			// Otherwise not a leap year
			else
			{
				if(dayOfMonth+daysForward > NON_LEAP_YEAR_MONTH_LENGTH[Month])
				{
					// Move backwards
					for(daysForward = MAX_DAYS_TO_GOOD_DAY; daysForward > 0; daysForward--)
					{
						daysSince2000 -= 1;
						if(IsGoodBusinessDay(daysSince2000))
							break;
					}
				}					
			}
		}
		else
		{
			// Empty else
		}
	}
	else
	{
		// Move backwards
		for(daysForward = MAX_DAYS_TO_GOOD_DAY; daysForward > 0; daysForward--)
		{
			daysSince2000 -= 1;
			if(IsGoodBusinessDay(daysSince2000))
				break;
		}
	}
	return daysSince2000;
}


// Given a date in the ISO format: YYYYMMDD convert it to the days since 2000
long ISODate::ConvertToDaysSince2000(long date)
{
	long year = date/10000;
	long monthAndDay = (date - year*10000);
	long monthNumber = monthAndDay/100;
	long dayOfMonth = monthAndDay - monthNumber*100;

	return DaysSince2000(dayOfMonth, monthNumber, year);
}


// Given a start date and end date return the number of years between the two dates assuming a 365 day year
double ISODate::YearFrac(long ISO_Start, long ISO_End)
{
	long startDays2000 = ConvertToDaysSince2000(ISO_Start);
	long endDays2000 = ConvertToDaysSince2000(ISO_End);

	return (endDays2000-startDays2000)/DAYS_PER_YEAR;
}
