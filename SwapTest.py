"""
This module creates a curve and builds a new 5 year swap and an existing swap
and prints out the float and fixed PVs. The existing swap and the new swap are
identical thus should give the same results. It also defines a function:
SanityCheck, this loops through the swap maturities loops through 5bp below the
swap rate to 5bp above the swap rate and prints out the PVs.  The PVs
should be 0 for the swap rate, they are close but I am not in a position to say
if they are close enough
"""

# The Python module that wraps the dll
import phin
# Load the module with the data in it
import data
# Load the module with GBP swaps
import GBPInstrument

# Get the data
libors = data.libors
swaps = data.swaps
fixedRate = data.fixedRate
valueDate = data.valueDate
maturityDate = data.maturityDate
settlementDate = data.settlementDate
currentRate = data.currentRate

maturityYears = 5

# Build the curve
c = phin.curve(valueDate,libors,swaps);
res = c.Build()


def BasicTest():
    # Build a new pay swap
    newSwap = GBPInstrument.GBPSwap(valueDate,maturityYears,fixedRate,True,c)
    print('Receiver Receive PV:', newSwap.pay_side_pv())
    print('Receiver Pay PV:', newSwap.receive_side_pv())
    print('Receiver PV:',newSwap.pv())

    # Build a new receive swap
    newSwap = GBPInstrument.GBPSwap(valueDate,maturityYears,fixedRate,False,c)
    print('Payer Receive PV:', newSwap.receive_side_pv())
    print('Payer Pay PV:', newSwap.pay_side_pv())
    print('Payer PV:',newSwap.pv())



def SanityCheck():
    """
    Checks swap PVs by looping through the maturities then looping through 5bp
    below the swap rate to 5bp above and printing out the PVs
    """
    bp = 0.0001

    # Iterate through the swaps
    for i,j in enumerate(swaps):
        for b in range(-10,11):
            s = GBPInstrument.GBPSwap(valueDate,i+1,j+b*bp,True,c)
            print(i+1,b,j+b*bp, s.pv())


def TestNewSwap():
    """
    Create a new swap and an existing swap with extaly the same inputs and
    calculate the pv.  The difference in the pv's should be 0
    """

    # Create and existing swap
    es = GBPInstrument.GBPExistingSwap(valueDate,maturityDate,fixedRate,currentRate,True,c)

    # Create a new swap
    ns = GBPInstrument.GBPSwap(valueDate,maturityYears,fixedRate,True,c)

    diffPV = ns.pv() - es.pv()

    print('This should be 0', diffPV)

    

