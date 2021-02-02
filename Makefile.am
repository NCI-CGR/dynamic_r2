bin_PROGRAMS = dynamic_r2.out

AM_CXXFLAGS = -I$(srcdir)/finter -I$(srcdir)/finter/include $(BOOST_CPPFLAGS) -ggdb -Wall -std=c++17

dynamic_r2_out_SOURCES = dynamic_r2/bin_calculator.cc dynamic_r2/bin_calculator.h dynamic_r2/cargs.cc dynamic_r2/cargs.h dynamic_r2/main.cc dynamic_r2/threshold_calculator.cc dynamic_r2/threshold_calculator.h dynamic_r2/utilities.cc dynamic_r2/utilities.h
dynamic_r2_out_LDADD = $(BOOST_LDFLAGS) -lfinter -lz -lbz2 -lboost_program_options
dist_doc_DATA = README
ACLOCAL_AMFLAGS = -I m4
