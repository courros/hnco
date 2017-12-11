noinst_PROGRAMS = \
	basic \
	stop-on-maximum \
	on-budget-function

AM_CXXFLAGS = $(OPENMP_CXXFLAGS)

AM_CPPFLAGS = -I $(top_srcdir)/lib

LDADD = ../lib/libhnco.la -ldl -lboost_serialization

basic_SOURCES = basic.cc
stop_on_maximum_SOURCES = stop-on-maximum.cc
on_budget_function_SOURCES = on-budget-function.cc
