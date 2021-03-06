#include "Curve.h"


// Constructor to build the class 
curve::curve(long ValueDate, std::vector<double>libors, std::vector<double>swaps)
{
	_ValueDate=ValueDate;
	SetLibors(libors);
	SetSwaps(swaps);
	return;
}


// Given the set of libors in the form of a std::vector make a deep copy to the class's private data member _libor
void curve::SetLibors(std::vector<double>libors)
{
	for(long i = 0; i < NUMBER_OF_LIBORS; i++)
	{
		_libors[i] = libors[i];
	}
	return;
}


// Given the set of swaps in the form of a std::vector make a deep copy to the class's private data member _swaps
void curve::SetSwaps(std::vector<double>swaps)
{
	for(long i = 0; i < NUMBER_OF_SWAPS; i++)
	{
		_swaps[i] = swaps[i];
	}
	return;
}


// Build the curve by calculating the discount factors and the numerical second derivatives for the discount factors, 
// the second derivatives are used in the cubic spline.  It does not check that the rates are set.
bool curve::Build()
{
	SetDate(_ValueDate);

	_DateArray[0] = DaysSince2000();
	_DF_Array[0] = 1;

	// Build up the Deposit end of the curve
	_DateArray[1] = DateIncrement(D,1);
	_DateArray[2] = DateIncrement(W,1);
	_DateArray[3] = DateIncrement(M,1);
	_DateArray[4] = DateIncrement(M,2);
	_DateArray[5] = DateIncrement(M,3);
	_DateArray[6] = DateIncrement(M,6);
	_DateArray[7] = DateIncrement(Y,1);

	// Generate the Df for the deposit curve
	for(long i = 1; i <= NUMBER_OF_LIBORS; i++)
	{
		_DF_Array[i] = 1/(1+_libors[i-1]*(_DateArray[i] - _DateArray[0])/DAYS_PER_YEAR);  
	}

	double dfSum = _DF_Array[NUMBER_OF_LIBORS];
	// Build up the dates and the DFs for the IRS section of the curve
	for(long i = 8; i < CURVE_LENGTH; i++)
	{
		_DateArray[i] = DateIncrement(Y, i-6);			// remove magic number pjc
		_DF_Array[i] = (1-_swaps[i-NUMBER_OF_LIBORS]*dfSum)/(1+_swaps[i-NUMBER_OF_LIBORS]);
		dfSum += _DF_Array[i];
	}

	//////////////////////// The Numerical Second Derivative Stuff Below this Point /////////////////////////////////
	yp1 = 1;
	ypn = 0;
	if(yp1 > 0.99e30)
	{
		_SecDerivDF[0] = u[0] = 0;
	}
	else
	{
		_SecDerivDF[0] = -0.5;
		u[0] = (3.0/(_DateArray[1] - _DateArray[0]))*((_DF_Array[1] - _DF_Array[0])/(_DateArray[1]-_DateArray[0]) - yp1);
	}

	// Now do the second derivative.
	for(long i = 1; i <= CURVE_LENGTH-2; i++)
	{
		sig = (_DateArray[i] - _DateArray[i-1])/(_DateArray[i+1] - _DateArray[i]);
		p = sig*_SecDerivDF[i-1] + 2;
		_SecDerivDF[i] = (sig-1)/p;
		u[i] = (_DF_Array[i+1]-_DF_Array[i])/(_DateArray[i+1]-_DateArray[i])-(_DF_Array[i]-_DF_Array[i-1])/(_DateArray[i]-_DateArray[i-1]);
		u[i] = (6*u[i]/(_DateArray[i+1]-_DateArray[i-1])-sig*u[i-1])/p;
	}

	qn = 0.0;
	un = (3/(_DateArray[CURVE_LENGTH-1]-_DateArray[CURVE_LENGTH-2]))*
		 (ypn-(_DF_Array[CURVE_LENGTH-1]-_DF_Array[CURVE_LENGTH-2])/(_DateArray[CURVE_LENGTH-1]-_DateArray[CURVE_LENGTH-2]));

	_SecDerivDF[CURVE_LENGTH-1] = (un-qn*u[CURVE_LENGTH-2])/(qn*_SecDerivDF[CURVE_LENGTH-2]+1);

	for(long k=CURVE_LENGTH-2; k>=0; k--)
	{
		_SecDerivDF[k]=_SecDerivDF[k]*_SecDerivDF[k+1]*u[k];
	}

	return true;
}


// Given a date in the days since 2000 format return the discount factor for the date.  It does no checking on the 
// input dates, it might return a Df for dates before the start of the df curve or dates beyond the the final date in
// the DF curve.  
double curve::GetDFFromDaysSince2000(long daysSince2000)
{
	double discountFactor = 0;

	long klo = 0;
	long khi = 0;
	long k = 0;

	double h = 0;
	double b = 0;
	double a = 0;

	klo = 0;
	khi = CURVE_LENGTH-1;

	while(khi-klo > 1)
	{
		k = (khi+klo) >> 1;
		if(_DateArray[k] > daysSince2000)
			khi = k;
		else
			klo = k;
	}

	h = _DateArray[khi] - _DateArray[klo];
	a = (_DateArray[khi]-daysSince2000)/h;
	b = (daysSince2000 - _DateArray[klo])/h;

	discountFactor = a*_DF_Array[klo] + b*_DF_Array[khi] +((a*a*a-a)*_SecDerivDF[klo] + (b*b*b-b)*_SecDerivDF[khi])*(h*h)/6;

	return discountFactor;
}


// Given a date in ISO format return the discount factor for that date.  Calls the function above.
double curve::GetDFFromISODate(long date)
{
	double discountFactor = 0;
	long daysSince2000 = 0;
	
	daysSince2000 = ConvertToDaysSince2000(date);
	discountFactor = GetDFFromDaysSince2000(daysSince2000);

	return discountFactor;
}


// Given two dates in days in since 2000 format return the linear annualized rate between those dates
// This function will error if endDF is 0.  This should never happen, at least with the accuracy of
// 32 bit floating point precision.  By handling the error here we are effectively swallowing the 
// excpetion, if endDF is 0 then there is a serious problem elsewhere
double curve::AnnualRateFromDate(long StartDate, long EndDate)
{
	double annualRate = 0;

	// Get the dfs
	double startDF = GetDFFromDaysSince2000(StartDate);
	double endDF = GetDFFromDaysSince2000(EndDate);
	
	// Convert to annual rate
	annualRate = (startDF/endDF-1)/((EndDate-StartDate)/DAYS_PER_YEAR);

	return annualRate;
}


// Given two dates in ISO format return the linear annualized rate between those dates
// Calls the function above
double curve::AnnualRateFromISODate(long StartDate, long EndDate)
{
	double annualRate = 0;

	// Covert dates from ISO format to days since 2000
	long StartDateDays = ConvertToDaysSince2000(StartDate);
	long EndDateDays = ConvertToDaysSince2000(EndDate);

	annualRate = AnnualRateFromDate(StartDateDays,EndDateDays);

	return annualRate;
}
