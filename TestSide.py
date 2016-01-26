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

# Build the curve
c = phin.curve(valueDate,libors,swaps);
res = c.Build()

sPayFloat = GBPInstrument.SideWrapper(20151022,c,False)

print('Pay Side PV', sPayFloat.PaySidePV())
print('Receive Side PV', sPayFloat.ReceiveSidePV())
print('PV', sPayFloat.PV())



payerSwap = GBPInstrument.GBPSwap(valueDate,5,fixedRate,True,c)
print('Payer Pay Side P and L:', payerSwap.PaySidePV())
print('Payer Receive Side P and L:', payerSwap.ReceiveSidePV())
print('Payer P and L:', payerSwap.PV())

receiverSwap= GBPInstrument.GBPSwap(valueDate,5,fixedRate,False,c)
print('Receiver Pay Side P and L:', receiverSwap.PaySidePV())
print('Receiver Receive Side P and L:', receiverSwap.ReceiveSidePV())
print('Receiver P and L:', receiverSwap.PV())

print('Payer Pay Side P and L:', payerSwap.PaySidePV())
print('Payer Receive Side P and L:', payerSwap.ReceiveSidePV())
print('Payer P and L:', payerSwap.PV())
