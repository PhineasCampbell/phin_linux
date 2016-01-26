all: swig_generate phin_wrap ISO_Dates Curve Library

# Python 2.7 path: /usr/include/python2.7
# Python 3.4 path: /usr/local/include/python3.4m
# /usr/src/Python-3.4.3/Include
# /usr/local/include/python3.4m


swig_generate:
	swig -version -Wall -c++ -python -cppext cpp phin.i

phin_wrap:
	g++ -Wall -fpic -c phin_wrap.cpp -I /usr/local/include/python3.4m

ISO_Dates:
	g++ -Wall -fpic -c ISODates.cpp

Curve:
	g++ -Wall -fpic -c Curve.cpp

Library:
	g++ -Wall -shared ISODates.o Curve.o phin_wrap.o -o _phin.so


	

