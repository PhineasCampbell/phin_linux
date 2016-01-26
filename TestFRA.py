import phin

"""
A quick and dirty module to check the FRA pricing object
"""

# Load the module with the data in it
import data
# Load the module with GBP swaps
import GBPInstrument


# Get the data
libors = data.libors
swaps = data.swaps
fixedRate = data.fixedRate
valueDate = data.valueDate
settlementDate = data.settlementDate

# Build the curve
c = phin.curve(valueDate,libors,swaps);
res = c.Build()

# Create the FRA
testFRA = GBPInstrument.FRA(valueDate,settlementDate,'3M',c)

print('FRA Rate:', testFRA.rate())


