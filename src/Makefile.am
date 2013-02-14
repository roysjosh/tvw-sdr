LIBVERSION = 0:0:0

tvwsdr_HEADERS = tvw-sdr.h

tvwsdrdir = $(includedir)

lib_LTLIBRARIES = libtvwsdr.la

libtvwsdr_la_SOURCES = libtvwsdr.c tda18271-common.c tda18271-fe.c tda18271-maps.c
libtvwsdr_la_LDFLAGS = -version-info $(LIBVERSION)

bin_PROGRAMS         = tvw_sdr

tvw_sdr_SOURCES      = tvw_sdr.c
tvw_sdr_LDADD        = libtvwsdr.la
