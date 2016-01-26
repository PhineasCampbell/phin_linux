#include "ISODates.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		This class builds a discount factor curve, it will return the discount factor for a date and the annualized
//		rate for the return between two dates
//		It's inputs are the LIBORs and swaps both of which are available on https://www.theice.com/iba/historical-data
//		It uses a linear rather than exponential cubic spline. The spline code is lifted almost verbatim from section 
//		3.3 of Numerical Recipies In C, I have made no attempt to understand this code. If you have problems with the 
//		code check the original source.  The date functionality comes from the ISODate class
//
//		The libors and swaps are passed into the class via the standard library vector objects, this has the advantage
//		that there is a map from the python list to the vector however it might limit the library being called from 
//		other environments.  Note on the ISO Date Format ISO 8601 defines YYYY-MM-DD or YYYYMMDD as acceptable however
//		this library only uses YYYYMMDD as it is easily representable as a long or integer thus can be passed between
//		various systems.
//
//		The code is pythonic in that it does not try to protect the users from him/herself, if you want to put stupid
//		things in you might get stupid things out
//		
//		To Do:	Either limit the call to getDf to 10 years to the future or extend the curve to infinity by adding
//				a very long term rate
//				Calibrate the numerical second derivatives for the spline
//				Extract the spline into a separate class
//				Give the option to use an exponential spline
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The number of LIBORs input into the class: ON, 1W, 1M, 2M, 3M, 6M, 12M
const long NUMBER_OF_LIBORS = 7;
// The number of swaps input into the class: 1yr, 2yr, 3yr, 4yr, 5yr, 6yr, 7yr, 8yr, 9yr, 10yr
const long NUMBER_OF_SWAPS = 10;
// The curve length
const long CURVE_LENGTH = NUMBER_OF_LIBORS + NUMBER_OF_SWAPS;


// Note private inheritance so as not to complicate the curve interface.
class curve: private ISODate
{
public:
	// Basic constructor
	curve(){return;}
	// Constructor to set the value date, libors and swaps
	curve(long valueDate, std::vector<double> libors, std::vector<double> swaps);
	// Set the LIBOR rates
	void SetLibors(std::vector<double> libors);
	// Set the swap rates
	void SetSwaps(std::vector<double> swaps);
	// Set the value date
	void SetValueDate(long valueDate){_ValueDate = valueDate;return;}
	// Build the curve, does not check that the libors, swaps or value date are set
	bool Build();
	// Given a date in ISO format the return the discount factor for this date
	double GetDFFromISODate(long date);
	// Given a date in the days from 2000 fromat return the discount factor for this date
	double GetDFFromDaysSince2000(long date);
	// Given a start date and an end date in the days since 2000 format return the linearized annual rate for the return 
	// between the two dates
	double AnnualRateFromDate(long startDate, long endDate);
	// Given a start date and an end date in the ISO format return the linearized annual rate for the return between 
	// the two dates
	double AnnualRateFromISODate(long startDate, long endDate);
private:
	// Array of libors
	double _libors[NUMBER_OF_LIBORS];
	// Array of swap rates
	double _swaps[NUMBER_OF_SWAPS];
	// Value date for the curve
	long _ValueDate;
	// Array of dates for the curve
	double _DateArray[CURVE_LENGTH];
	// Array of discount factors for the dates above
	double _DF_Array[CURVE_LENGTH];
	// Array of numerical second derivatives for the curve
	double _SecDerivDF[CURVE_LENGTH];
	///////////////////////////////////////////// Spline Stuff ////////////////////////////////////////////////////////
	double u[CURVE_LENGTH-2];
	double p;
	double qn;
	double sig;
	double un;
	double yp1;
	double ypn;
};