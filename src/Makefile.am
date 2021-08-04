## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
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
	cpu-set-manager.cc \
	cpu-set-manager.h \
	common.h \
	judger.cc \
	judger.h \
	remote-object.cc \
	remote-object.h \
	problem.cc \
	problem.h \
	http.cc \
	http.h \
	json.h \
	json-forwards.h \
	jsoncpp.cpp \
	solutions-listener.cc \
	solutions-listener.h \
	judge-worker.cc \
	judge-worker.h \
	solution.cc \
	solution.h \
	sand-box.cc \
	sand-box.h

wzoj_judger2_CXXFLAGS = -lpthread 

wzoj_judger2_LDFLAGS = 

wzoj_judger2_LDADD = $(WZOJ_JUDGER2_LIBS) \
	$(libcurl_LIBS) \
	$(LIBCURL_LIBS)


