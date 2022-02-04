## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(hiredis_CFLAGS). \
	$(LIBCURL_CFLAGS). \
	$(libcurl_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(WZOJ_JUDGER2_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = wzoj_judger2

wzoj_judger2_SOURCES = \
	main.cc \
	common.h \
	wjudger.pb.h \
	wjudger.pb.cc \
	wjudger.grpc.pb.h \
	wjudger.grpc.pb.cc \
	w-server.cc \
	w-server.h

wzoj_judger2_CXXFLAGS = -lpthread 

wzoj_judger2_LDFLAGS = 

wzoj_judger2_LDADD = $(WZOJ_JUDGER2_LIBS)

