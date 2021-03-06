# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.8
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_phin', [dirname(__file__)])
        except ImportError:
            import _phin
            return _phin
        if fp is not None:
            try:
                _mod = imp.load_module('_phin', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _phin = swig_import_helper()
    del swig_import_helper
else:
    import _phin
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        if _newclass:
            object.__setattr__(self, name, value)
        else:
            self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



_phin.D_swigconstant(_phin)
D = _phin.D

_phin.W_swigconstant(_phin)
W = _phin.W

_phin.M_swigconstant(_phin)
M = _phin.M

_phin.Y_swigconstant(_phin)
Y = _phin.Y

_phin.P_swigconstant(_phin)
P = _phin.P

_phin.F_swigconstant(_phin)
F = _phin.F

_phin.MF_swigconstant(_phin)
MF = _phin.MF

_phin.N_swigconstant(_phin)
N = _phin.N
class ISODate(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ISODate, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ISODate, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        this = _phin.new_ISODate(*args)
        try:
            self.this.append(this)
        except Exception:
            self.this = this
    __swig_destroy__ = _phin.delete_ISODate
    __del__ = lambda self: None

    def SetDateAdjustment(self, adjustmentType):
        return _phin.ISODate_SetDateAdjustment(self, adjustmentType)

    def DaysSince2000(self, *args):
        return _phin.ISODate_DaysSince2000(self, *args)

    def SetDate(self, *args):
        return _phin.ISODate_SetDate(self, *args)

    def DateIncrement(self, dp, number, returnAsDaysSince2000=True):
        return _phin.ISODate_DateIncrement(self, dp, number, returnAsDaysSince2000)

    def IsLeapYear(self, Year):
        return _phin.ISODate_IsLeapYear(self, Year)

    def IsGoodBusinessDay(self, daysSince2000):
        return _phin.ISODate_IsGoodBusinessDay(self, daysSince2000)

    def DaysToGoodBusinessDay(self, daysSince2000, moveForward=True):
        return _phin.ISODate_DaysToGoodBusinessDay(self, daysSince2000, moveForward)

    def DateSince2000(self, daysSince2000):
        return _phin.ISODate_DateSince2000(self, daysSince2000)

    def NextGoodBusinessDay(self, dayOfMonth, Month, Year, arg5=MF):
        return _phin.ISODate_NextGoodBusinessDay(self, dayOfMonth, Month, Year, arg5)

    def IsBankHoliday(self, daysSince2000):
        return _phin.ISODate_IsBankHoliday(self, daysSince2000)

    def ConvertToDaysSince2000(self, date):
        return _phin.ISODate_ConvertToDaysSince2000(self, date)

    def ConvertToISO(self, dayOfMonth, Month, Year):
        return _phin.ISODate_ConvertToISO(self, dayOfMonth, Month, Year)

    def ReturnAsISO(self):
        return _phin.ISODate_ReturnAsISO(self)

    def YearFrac(self, ISO_Start, ISO_End):
        return _phin.ISODate_YearFrac(self, ISO_Start, ISO_End)
ISODate_swigregister = _phin.ISODate_swigregister
ISODate_swigregister(ISODate)

class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _phin.delete_SwigPyIterator
    __del__ = lambda self: None

    def value(self):
        return _phin.SwigPyIterator_value(self)

    def incr(self, n=1):
        return _phin.SwigPyIterator_incr(self, n)

    def decr(self, n=1):
        return _phin.SwigPyIterator_decr(self, n)

    def distance(self, x):
        return _phin.SwigPyIterator_distance(self, x)

    def equal(self, x):
        return _phin.SwigPyIterator_equal(self, x)

    def copy(self):
        return _phin.SwigPyIterator_copy(self)

    def next(self):
        return _phin.SwigPyIterator_next(self)

    def __next__(self):
        return _phin.SwigPyIterator___next__(self)

    def previous(self):
        return _phin.SwigPyIterator_previous(self)

    def advance(self, n):
        return _phin.SwigPyIterator_advance(self, n)

    def __eq__(self, x):
        return _phin.SwigPyIterator___eq__(self, x)

    def __ne__(self, x):
        return _phin.SwigPyIterator___ne__(self, x)

    def __iadd__(self, n):
        return _phin.SwigPyIterator___iadd__(self, n)

    def __isub__(self, n):
        return _phin.SwigPyIterator___isub__(self, n)

    def __add__(self, n):
        return _phin.SwigPyIterator___add__(self, n)

    def __sub__(self, *args):
        return _phin.SwigPyIterator___sub__(self, *args)
    def __iter__(self):
        return self
SwigPyIterator_swigregister = _phin.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

class DoubleVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, DoubleVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, DoubleVector, name)
    __repr__ = _swig_repr

    def iterator(self):
        return _phin.DoubleVector_iterator(self)
    def __iter__(self):
        return self.iterator()

    def __nonzero__(self):
        return _phin.DoubleVector___nonzero__(self)

    def __bool__(self):
        return _phin.DoubleVector___bool__(self)

    def __len__(self):
        return _phin.DoubleVector___len__(self)

    def __getslice__(self, i, j):
        return _phin.DoubleVector___getslice__(self, i, j)

    def __setslice__(self, *args):
        return _phin.DoubleVector___setslice__(self, *args)

    def __delslice__(self, i, j):
        return _phin.DoubleVector___delslice__(self, i, j)

    def __delitem__(self, *args):
        return _phin.DoubleVector___delitem__(self, *args)

    def __getitem__(self, *args):
        return _phin.DoubleVector___getitem__(self, *args)

    def __setitem__(self, *args):
        return _phin.DoubleVector___setitem__(self, *args)

    def pop(self):
        return _phin.DoubleVector_pop(self)

    def append(self, x):
        return _phin.DoubleVector_append(self, x)

    def empty(self):
        return _phin.DoubleVector_empty(self)

    def size(self):
        return _phin.DoubleVector_size(self)

    def swap(self, v):
        return _phin.DoubleVector_swap(self, v)

    def begin(self):
        return _phin.DoubleVector_begin(self)

    def end(self):
        return _phin.DoubleVector_end(self)

    def rbegin(self):
        return _phin.DoubleVector_rbegin(self)

    def rend(self):
        return _phin.DoubleVector_rend(self)

    def clear(self):
        return _phin.DoubleVector_clear(self)

    def get_allocator(self):
        return _phin.DoubleVector_get_allocator(self)

    def pop_back(self):
        return _phin.DoubleVector_pop_back(self)

    def erase(self, *args):
        return _phin.DoubleVector_erase(self, *args)

    def __init__(self, *args):
        this = _phin.new_DoubleVector(*args)
        try:
            self.this.append(this)
        except Exception:
            self.this = this

    def push_back(self, x):
        return _phin.DoubleVector_push_back(self, x)

    def front(self):
        return _phin.DoubleVector_front(self)

    def back(self):
        return _phin.DoubleVector_back(self)

    def assign(self, n, x):
        return _phin.DoubleVector_assign(self, n, x)

    def resize(self, *args):
        return _phin.DoubleVector_resize(self, *args)

    def insert(self, *args):
        return _phin.DoubleVector_insert(self, *args)

    def reserve(self, n):
        return _phin.DoubleVector_reserve(self, n)

    def capacity(self):
        return _phin.DoubleVector_capacity(self)
    __swig_destroy__ = _phin.delete_DoubleVector
    __del__ = lambda self: None
DoubleVector_swigregister = _phin.DoubleVector_swigregister
DoubleVector_swigregister(DoubleVector)

class curve(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, curve, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, curve, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        this = _phin.new_curve(*args)
        try:
            self.this.append(this)
        except Exception:
            self.this = this

    def SetLibors(self, libors):
        return _phin.curve_SetLibors(self, libors)

    def SetSwaps(self, swaps):
        return _phin.curve_SetSwaps(self, swaps)

    def SetValueDate(self, valueDate):
        return _phin.curve_SetValueDate(self, valueDate)

    def Build(self):
        return _phin.curve_Build(self)

    def GetDFFromISODate(self, date):
        return _phin.curve_GetDFFromISODate(self, date)

    def GetDFFromDaysSince2000(self, date):
        return _phin.curve_GetDFFromDaysSince2000(self, date)

    def AnnualRateFromDate(self, startDate, endDate):
        return _phin.curve_AnnualRateFromDate(self, startDate, endDate)

    def AnnualRateFromISODate(self, startDate, endDate):
        return _phin.curve_AnnualRateFromISODate(self, startDate, endDate)
    __swig_destroy__ = _phin.delete_curve
    __del__ = lambda self: None
curve_swigregister = _phin.curve_swigregister
curve_swigregister(curve)

# This file is compatible with both classic and new-style classes.


