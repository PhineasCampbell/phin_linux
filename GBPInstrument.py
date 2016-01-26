import phin

"""
This module exports a GBP interest rate derivatives currently only a GBP IRS and a GBP FRA.  
"""

# To Do:
#    Check FRA, It has been a while since I have had to calulate the value of an FRA I am not sure the arithmetic is
#    100% correct.  I have checked the output and it appears to be correct. Even if it is there is an algebraic simplification
#    however including the convexity term makes the methodology clearer
#
#    Tests for the two instruments can be found in SwapTest.py and TestFRA.py


MAX_SEMI_ANNUAL_YEARS = 21
"""
The class will not deal with swaps of greater maturity than 10 years which
corresponds to 21 payments
"""

SUPPORTED_TENORS = {'ON':(phin.D,1),'1W':(phin.D,7),'1M':(phin.M,1),'2M':(phin.M,2),'3M':(phin.M,3),'6M':(phin.M,6),'12M':(phin.M,12),'1Y':(phin.Y,1)}
"""
A Dictionary that maps the available LIBOR tenors to the inputs into DateIncrement.
If there are enough of these inputs it might justify a mixin. Warning in order to
generate schedules 1Y is included but the equivalent LIBOR tenor as 12M
"""


class Side(phin.ISODate):
    """
    This class is the side of a swap.  It can be either pay or receive and fixed or
    float.  It is important that the structure is able to capture swaps that pay and
    receive fixed or pay and receive float. 
    """                
    def __init__(self, valueDate, maturity, rate, c, tenor, fixed = True, pay = True):
        """
        Create a new side. Input the value date in YYYYMMDD form, the number of
        years to maturity, the fixed rate, a curve, the tenor of the float side
        eg '6M', fixed = True float = False, pay = True, receive = False 
        """
        
        # The first thing we are going to do is extract the tenor
        try:
            self._tenorDate = SUPPORTED_TENORS[tenor]
        except KeyError:
            print('Incorrect Tenor')
            return

        self._pay = pay
        self._fixed = fixed
        self._cashFlows = dict()
        self._curve = c

        # A ISODate object to generate schedule dates
        ds = phin.ISODate(valueDate)

        if pay:
            self._polarity = -1
        else:
            self._polarity = 1

        oldDate = valueDate

        # Split the fixed and float cases. There is code duplication here so may be a case for refactoring
        if fixed:
            # Build up the schedules 
            for i in range(2*maturity+1):
                payDate = ds.DateIncrement(self._tenorDate[0],self._tenorDate[1]*i,False)
                # There is no flow on value date
                if payDate ==  valueDate:
                    self._cashFlows[payDate] = 0
                else:
                    self._cashFlows[payDate] = self._polarity*rate*ds.YearFrac(oldDate,payDate)
                #print(oldDate,i,rate,ds.YearFrac(oldDate,i))
                oldDate = payDate
        else: # Must be float
            # Build up the schedules 
            for i in range(2*maturity+1):
                payDate = ds.DateIncrement(self._tenorDate[0],self._tenorDate[1]*i,False)
                # There is no flow on value date
                if payDate ==  valueDate:
                    self._cashFlows[payDate] = 0
                else:
                    self._cashFlows[payDate] = self._polarity*self._curve.AnnualRateFromISODate(oldDate,payDate)*ds.YearFrac(oldDate,payDate)
                oldDate = payDate

    def pv(self):
        """
        Return the PV for the cash flow using the curve defined in self._curve
        """
        pvPay = 0
        
        for i,j in self._cashFlows.items():
            pvPay += j*self._curve.GetDFFromISODate(i)
        return pvPay;


class BackwardSide(phin.ISODate):
    """
    This class is the side of a swap.  It can be either pay or receive and fixed or
    float.  It is important that the structure is able to capture swaps that pay and
    receive fixed or pay and receive float. 
    """                
    def __init__(self, valueDate, maturityDate, rate, c, tenor, currentRate, fixed = True, pay = True):
        """
        Create a new side. Input the value date in YYYYMMDD form, the number of
        years to maturity, the fixed rate, a curve, the tenor of the float side
        eg '6M', fixed = True float = False, pay = True, receive = False 
        """
        
        # The first thing we are going to do is extract the tenor
        try:
            self._tenorDate = SUPPORTED_TENORS[tenor]
        except KeyError:
            print('Incorrect Tenor')
            return

        self._pay = pay
        self._fixed = fixed
        self._cashFlows = dict()
        self._curve = c

        if pay:
            self._polarity = -1
        else:
            self._polarity = 1

        # A ISODate object to generate schedule dates
        ds = phin.ISODate(maturityDate)

        # Build up the schedules 
        for i in range(0,-1*MAX_SEMI_ANNUAL_YEARS,-1):
            payDate = ds.DateIncrement(self._tenorDate[0],self._tenorDate[1]*i,False)
            self._cashFlows[payDate] = 0
            if payDate <= valueDate:
                break

        # Given we have a schedule we can now build up the cashflows
        # Split the fixed and float cases. There is code duplication here so may be a case for refactoring
        sortedKeys = sorted(self._cashFlows.keys())
        if fixed:
            for i in sortedKeys:
                # There are no payments before the value date
                if i <= valueDate:
                    self._cashFlows[i] = 0
                    oldDate = i
                else:
                    self._cashFlows[i] = self._polarity*rate*ds.YearFrac(oldDate,i)
                    oldDate = i                
        # Otherwise must be float
        else:
            for j,i in enumerate(sortedKeys):
                #There are no payments before the value date
                if i <= valueDate:
                    self._cashFlows[i] = 0
                    oldDate = i
                else:
                    # If this is the first payment then use the existing rate
                    if(j == 1):
                        self._cashFlows[i] = self._polarity*currentRate*ds.YearFrac(oldDate,i)
                    else:
                        self._cashFlows[i] = self._polarity*self._curve.AnnualRateFromISODate(oldDate,i)*ds.YearFrac(oldDate,i)
                    oldDate = i                
            
    def pv(self):
        """
        Return the PV for the cash flow using the curve defined in self._curve
        """
        pvPay = 0
        for i,j in self._cashFlows.items():
            pvPay += j*self._curve.GetDFFromISODate(i)
        return pvPay;


class GBPSwap(Side):
    """
    This is a class that implements a standard GBP fixed float swap.  It
    simpifies the interface by making it a semi annual fixed float swap
    """
    _payFixed = True
    _receiveFixed = False

    def __init__(self,valueDate,maturity,rate,pay,c):
        """
        Input the value date, the years to maturity, fixed rate, pay or receive True = Pay
        and a curve to create a GBP IRS
        """
        self._valueDate = valueDate
        self._maturity = maturity
        self._rate = rate

        if not pay:
            self._payFixed = False
            self._receiveFixed = True

        self._curve = c     
        self._paySide = Side(self._valueDate,self._maturity,self._rate,self._curve,'6M',self._payFixed, True)
        self._receiveSide = Side(self._valueDate,self._maturity,self._rate,self._curve,'6M',self._receiveFixed, False)

    def pay_side_pv(self):
        """
        Return the pv of the pay side 
        """
        return self._paySide.pv()

    def receive_side_pv(self):
        """
        Return the pv of the receive side
        """
        return self._receiveSide.pv()

    def pv(self):
        """
        Return the pv of the swap
        """
        return self._paySide.pv() + self._receiveSide.pv()
    

class GBPExistingSwap(BackwardSide):
    """
    A class to capture the behaviour of an existing swap
    """
    _valueDate = 0
    _maturityDate = 0
    _fixedRate = 0
    _currentFloatRate = 0

    _payFixed = True
    _receiveFixed = False

    def __init__(self, valueDate, maturityDate, fixedRate, currentFloatRate, pay, c):
        """
        Create an instance of a existing swap by passing in the value date,
        maturity date, fixed rate, current float rate and a curve
        """
        self._valueDate = valueDate
        self._maturityDate = maturityDate
        self._fixedRate = fixedRate
        self._currentFloatRate = currentFloatRate
        self._curve = c
        self._payFixed = True

        if not pay:
            self._payFixed = False
            self._receiveFixed = True

        # Create the pay and receive sides
        self._paySide =     BackwardSide(self._valueDate,self._maturityDate,self._fixedRate,self._curve,'6M',self._currentFloatRate,self._payFixed, True)
        self._receiveSide = BackwardSide(self._valueDate,self._maturityDate,self._fixedRate,self._curve,'6M',self._currentFloatRate,False,False)

    def pay_side_pv(self):
        return self._paySide.pv()

    def receive_side_pv(self):
        return self._receiveSide.pv()

    def pv(self):
        return self._paySide.pv() + self._receiveSide.pv()
                      

class FRA(phin.ISODate):
    """
    A standard GBP FRA the inputs are a value date, a settlement date, a tenor
    e.g. 3M and a GBP curve.
    """
    _valueDate = 0
    _settlementDate = 0
    _maturityDate = 0
    _tenor = 0
    # The _tenorDate is a tuple that contains the month unit eg 'M' and the number of these required to get to maturity
    _tenorDate = ()    

    def __init__(self, valueDate, settlementDate, tenor, c):
        """
        Create an FRA by inputing the value date, settlement date, tenor and
        curve to create a GBP FRA
        """ 
        try:
            self._tenorDate = SUPPORTED_TENORS[tenor]
        except KeyError:
            print('Incorrect Tenor')
            return
        self._valueDate = valueDate
        self._settlementDate = settlementDate
        self._curve = c
        # Create an ISODate object to control date creation
        self._dg = phin.ISODate(self._settlementDate)
        self._maturityDate = self._dg.DateIncrement(self._tenorDate[0],self._tenorDate[1],False)

    def rate(self):
        """
        Return the par FRA rate for the given curve
        """
        # Get the forward rate
        forwardRate = self._curve.AnnualRateFromISODate(self._settlementDate,self._maturityDate)
        
        # Get the forward discount factors
        dfAtSettlement = self._curve.GetDFFromISODate(self._settlementDate)
        dfAtMaturity = self._curve.GetDFFromISODate(self._maturityDate)

        # Now calculate the convexity adjustment
        convexityAdjustment = dfAtMaturity/dfAtSettlement

        rate = forwardRate*convexityAdjustment

        return rate

