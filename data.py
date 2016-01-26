"""
This file contains the swap and libor data for the 22 October 2015
"""


valueDate = 20151022

libors = list()
libors.append(0.004825)  
libors.append(0.0048656) 
libors.append(0.0050756) 
libors.append(0.0053913) 
libors.append(0.0057938) 
libors.append(0.007425)  
libors.append(0.010315)  

swaps = list()
swaps.append(0.00655)
swaps.append(0.00946)
swaps.append(0.01116)
swaps.append(0.01278)
swaps.append(0.01423)
swaps.append(0.01545)
swaps.append(0.01646)
swaps.append(0.01729)
swaps.append(0.018)
swaps.append(0.01861)


fixedRate = 0.01423
currentRate = 0.007425

maturityDate = 20201022
settlementDate = 20160122


trades = {}

trades[101] = {}
trades[102] = {}

trades[101]['maturiyDate'] = 20171030
trades[101]['fixedRate'] = 0.035
trades[101]['payOrReceive'] = True
trades[101]['currentFloatRate']= 0.012

trades[102]['maturiyDate'] = 20171030
trades[102]['fixedRate'] = 0.035
trades[102]['payOrReceive'] = False
trades[102]['currentFloatRate']= 0.012



          


