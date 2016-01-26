/* File phin.i */

/* The interface file used by swig to generate the Python wrappers for the c++ 
   classes ISODates and curve
*/


/* this is the module name as in import x */
%module phin

/* To simplfy the interface we exclude some constants */
%ignore NUMBER_OF_LIBORS;
%ignore MAX_DAYS_TO_GOOD_DAY;
%ignore BANK_HOLIDAY_COUNT;
%ignore NON_LEAP_YEAR_MONTH_DAYS;
%ignore LEAP_YEAR_MONTH_DAYS;
%ignore NON_LEAP_YEAR_MONTH_LENGTH;
%ignore LEAP_YEAR_MONTH_LENGTH;
%ignore GBP_HOLIDAYS;
%ignore MAX_YEARS;
%ignore DAYS_PER_YEAR;
%ignore DAYS_PER_LEAP_YEAR;
%ignore NUMBER_OF_LIBORS;
%ignore NUMBER_OF_SWAPS;
%ignore CURVE_LENGTH;

%{
#include "Curve.h" 
%}

/* excluded as all the definitions are in Curve.h
%{
#include "ISODates.h"
%}
*/

/* generate exported functions using the header file */
%include "ISODates.h"

%include stl.i
/*instantiate the required template specializations*/
namespace std {
    %template(DoubleVector) vector<double>;
}

/* generate exported functions using the header file */
%include "Curve.h"  /*this just expands out for the interface*/

/* This just gets copied to the wrap_file */
%{
/* does this get copied */
%}

