#
# InfoSel++ Project
# Build & Distribute
# GNU Makefile
#

prjdir=../../prj
outdir=./out
distdir=../../dist
bindir=$(distdir)/bin
libdir=$(distdir)/lib
includedir=$(distdir)/include
docdir=$(distdir)/doc
makedir=$(distdir)/make
srcdir=$(distdir)/src
testdir=$(distdir)/test
workdir=$(distdir)/work
exesuf=.g.exe

CXX=g++
CXXFLAGS=-w -O2 -ffast-math -I$(prjdir)/library/kernel/inc/core -I$(prjdir)/library/kernel/inc/api -I$(prjdir)/library/kernel/inc/ui -I$(prjdir)/library/frontend/inc
AR=ar
ARFLAGS=rcs
LD=gcc
LDFLAGS=-lstdc++ -lm

default: docs apps
	@echo done targets: $^.

all: libs incs docs apps tests samples
	@echo done all targets.

libs: $(libdir)/libinfosel++.a \
      $(libdir)/libiselpacks.a \
      $(libdir)/libiselconsl.a 
	@echo ===== Target \'libs\' build: 1>&2
	@echo prerequisites: $^ 1>&2
	@echo no errors or up to date 1>&2

incs: $(subst $(prjdir)/library/kernel/inc/core,$(includedir),$(shell ls $(prjdir)/library/kernel/inc/core/*.h)) \
      $(subst $(prjdir)/library/kernel/inc/api,$(includedir),$(shell ls $(prjdir)/library/kernel/inc/api/*.h)) \
      $(subst $(prjdir)/library/frontend/inc,$(includedir),$(shell ls $(prjdir)/library/frontend/inc/*.h))
	@echo ===== Target \'incs\' build: 1>&2
	@echo prerequisites: $^ 1>&2
	@echo no errors or up to date 1>&2

docs: $(docdir)/infosel-tutorial.ps \
      $(docdir)/infosel-tutorial.pdf
	@echo ===== Target \'docs\' build: 1>&2
	@echo prerequisites: $^ 1>&2
	@echo no errors or up to date 1>&2

apps: $(bindir)/infosel++$(exesuf) \
      $(workdir)/control.ipp $(workdir)/input.dat $(workdir)/_run.sh
	@echo ===== Target \'apps\' build: 1>&2
	@echo prerequisites: $^ 1>&2
	@echo no errors or up to date 1>&2

tests: $(bindir)/iseltest_api$(exesuf) \
       $(bindir)/iseltest_ui$(exesuf) \
       $(bindir)/iseltest_fe$(exesuf) \
       $(testdir)/alltest.ipp $(testdir)/fetest.ipp $(testdir)/alltest.dat $(testdir)/apitest.dat $(testdir)/fetest.dat $(testdir)/uitest.dat $(testdir)/_alltest.sh $(testdir)/_apitest.sh $(testdir)/_fetest.sh $(testdir)/_uitest.sh
	@echo ===== Target \'tests\' build: 1>&2
	@echo prerequisites: $^ 1>&2
	@echo no errors or up to date 1>&2

samples: $(makedir)/_clean.sh $(makedir)/_mkbat.sh $(makedir)/_mkcln.sh $(makedir)/_mkcon.sh $(makedir)/_mkshl.sh \
         $(workdir)/abc.ipp $(workdir)/abc.dat $(workdir)/_exec.sh \
         $(subst $(prjdir)/libapps/samples,$(srcdir),$(shell ls $(prjdir)/libapps/samples/*.cpp)) \
         $(subst $(prjdir)/libpacks,$(srcdir),$(shell ls $(prjdir)/libpacks/*.cxx))
	@echo ===== Target \'samples\' build: 1>&2
	@echo prerequisites: $^ 1>&2
	@echo no errors or up to date 1>&2

$(libdir)/%.a : $(outdir)/%.a
	@echo ===== All archive build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(bindir)/infosel++$(exesuf) : $(outdir)/libiselconsl.a \
                               $(subst $(prjdir)/libpacks,$(outdir),$(subst .cxx,.o,$(shell ls $(prjdir)/libpacks/*.cxx))) \
                               $(outdir)/libiselpacks.a \
                               $(outdir)/libinfosel++.a
	@echo ===== Console application build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo linking $@ ...
	@$(LD) $^ $(LDFLAGS) -o $@
	@-chmod a+x $@
	@echo no errors 1>&2

$(workdir)/%.dat : $(prjdir)/libapps/stdcon/%.dat
	@echo ===== Console application build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(workdir)/%.ipp : $(prjdir)/libapps/stdcon/%.ipp
	@echo ===== Console application build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(workdir)/%.sh : $(prjdir)/libapps/stdcon/%.sh
	@echo ===== Console application build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@-chmod a+x $@
	@echo no errors 1>&2

$(bindir)/iseltest_api$(exesuf) : $(subst $(prjdir)/libtests/api,$(outdir),$(subst .cpp,.o,$(shell ls $(prjdir)/libtests/api/*.cpp))) \
                                  $(outdir)/libinfosel++.a
	@echo ===== Test program build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo linking $@ ...
	@$(LD) $^ $(LDFLAGS) -o $@ 
	@-chmod a+x $@
	@echo no errors 1>&2

$(bindir)/iseltest_ui$(exesuf) : $(subst $(prjdir)/libtests/ui,$(outdir),$(subst .cpp,.o,$(shell ls $(prjdir)/libtests/ui/*.cpp))) \
                                 $(outdir)/libinfosel++.a
	@echo ===== Test program build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo linking $@ ...
	@$(LD) $^ $(LDFLAGS) -o $@ 
	@-chmod a+x $@
	@echo no errors 1>&2

$(bindir)/iseltest_fe$(exesuf) : $(subst $(prjdir)/libtests/fe,$(outdir),$(subst .cpp,.o,$(shell ls $(prjdir)/libtests/fe/*.cpp))) \
                                 $(outdir)/libinfosel++.a
	@echo ===== Test program build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo linking $@ ...
	@$(LD) $^ $(LDFLAGS) -o $@ 
	@-chmod a+x $@
	@echo no errors 1>&2

$(outdir)/libinfosel++.a : $(subst $(prjdir)/library/kernel/src,$(outdir),$(subst .cpp,.o,$(shell ls $(prjdir)/library/kernel/src/*.cpp))) \
                           $(subst $(prjdir)/library/frontend/src,$(outdir),$(subst .cpp,.o,$(shell ls $(prjdir)/library/frontend/src/*.cpp)))
	@echo ===== Library archive build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo archiving $@ ...
	@$(AR) $(ARFLAGS) $@ $^
	@echo no errors 1>&2

$(outdir)/libiselpacks.a : $(subst $(prjdir)/libpacks,$(outdir),$(subst .cpp,.o,$(shell ls $(prjdir)/libpacks/*.cpp)))
	@echo ===== Package archive build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo archiving $@ ...
	@$(AR) $(ARFLAGS) $@ $^
	@echo no errors 1>&2

$(outdir)/libiselconsl.a : $(subst $(prjdir)/libapps/stdcon,$(outdir),$(subst .cpp,.o,$(shell ls $(prjdir)/libapps/stdcon/*.cpp)))
	@echo ===== Console archive build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo archiving $@ ...
	@$(AR) $(ARFLAGS) $@ $^
	@echo no errors 1>&2

$(outdir)/%.o : $(prjdir)/library/kernel/src/%.cpp \
                $(shell ls $(prjdir)/library/kernel/inc/core/*.h $(prjdir)/library/kernel/inc/api/*.h $(prjdir)/library/kernel/inc/ui/*.h)
	@echo ===== Library object build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2

$(outdir)/%.o : $(prjdir)/library/frontend/src/%.cpp \
                $(shell ls $(prjdir)/library/kernel/inc/core/*.h $(prjdir)/library/kernel/inc/ui/*.h $(prjdir)/library/frontend/inc/*.h)
	@echo ===== Library object build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2

$(outdir)/%.o : $(prjdir)/libpacks/%.cpp \
                $(shell ls $(prjdir)/library/kernel/inc/core/*.h $(prjdir)/library/kernel/inc/api/*.h)
	@echo ===== Package object build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2

$(outdir)/%.o : $(prjdir)/libpacks/%.cxx \
                $(shell ls $(prjdir)/library/frontend/inc/*.h)
	@echo ===== Package config build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2

$(outdir)/%.o : $(prjdir)/libapps/stdcon/%.cpp \
                $(shell ls $(prjdir)/library/kernel/inc/core/*.h $(prjdir)/library/kernel/inc/ui/*.h $(prjdir)/library/frontend/inc/*.h)
	@echo ===== Console object build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2
	
$(outdir)/%.o : $(prjdir)/libtests/api/%.cpp \
                $(shell ls $(prjdir)/library/kernel/inc/core/*.h $(prjdir)/library/kernel/inc/api/*.h $(prjdir)/library/kernel/inc/ui/*.h)
	@echo ===== Test object build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2
	
$(outdir)/%.o : $(prjdir)/libtests/ui/%.cpp \
                $(shell ls $(prjdir)/library/kernel/inc/core/*.h $(prjdir)/library/kernel/inc/api/*.h $(prjdir)/library/kernel/inc/ui/*.h)
	@echo ===== Test object build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2
	
$(outdir)/%.o : $(prjdir)/libtests/fe/%.cpp \
                $(shell ls $(prjdir)/library/kernel/inc/core/*.h $(prjdir)/library/kernel/inc/api/*.h $(prjdir)/library/frontend/inc/*.h)
	@echo ===== Test object build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo compiling $@ ...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo no errors 1>&2
	
$(includedir)/%.h : $(prjdir)/library/kernel/inc/core/%.h
	@echo ===== Library header build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(includedir)/%.h : $(prjdir)/library/kernel/inc/api/%.h
	@echo ===== Library header build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(includedir)/%.h : $(prjdir)/library/frontend/inc/%.h
	@echo ===== Library header build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(docdir)/infosel-tutorial.% : $(prjdir)/libdocs/tutorial/infosel.%
	@echo ===== Library tutorial build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(srcdir)/%.cpp : $(prjdir)/libapps/samples/%.cpp
	@echo ===== Library sample build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(srcdir)/%.cxx : $(prjdir)/libpacks/%.cxx
	@echo ===== Library sample build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(makedir)/%.sh : $(prjdir)/libapps/samples/%.sh
	@echo ===== Library sample build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@-chmod a+x $@
	@echo no errors 1>&2

$(workdir)/%.dat : $(prjdir)/libapps/samples/%.dat
	@echo ===== Library sample build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(workdir)/%.ipp : $(prjdir)/libapps/samples/%.ipp
	@echo ===== Library sample build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(workdir)/%.sh : $(prjdir)/libapps/samples/%.sh
	@echo ===== Library sample build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@-chmod a+x $@
	@echo no errors 1>&2

$(testdir)/%.dat : $(prjdir)/libtests/all/%.dat
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(testdir)/%.ipp : $(prjdir)/libtests/all/%.ipp
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(testdir)/%.sh : $(prjdir)/libtests/all/%.sh
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@-chmod a+x $@
	@echo no errors 1>&2

$(testdir)/%.dat : $(prjdir)/libtests/api/%.dat
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(testdir)/%.sh : $(prjdir)/libtests/api/%.sh
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@-chmod a+x $@
	@echo no errors 1>&2

$(testdir)/%.dat : $(prjdir)/libtests/fe/%.dat
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(testdir)/%.ipp : $(prjdir)/libtests/fe/%.ipp
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(testdir)/%.sh : $(prjdir)/libtests/fe/%.sh
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@-chmod a+x $@
	@echo no errors 1>&2

$(testdir)/%.dat : $(prjdir)/libtests/ui/%.dat
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@echo no errors 1>&2

$(testdir)/%.sh : $(prjdir)/libtests/ui/%.sh
	@echo ===== Library test build: 1>&2
	@echo target: $@ 1>&2
	@echo prerequisites: $^ 1>&2
	@echo copying $@ ...
	@cp $< $@
	@-chmod a+x $@
	@echo no errors 1>&2

clean:
	@echo ===== Target 'clean' build: 1>&2
	@echo deleting all build files ...
	@-rm $(libdir)/libinfosel++.a
	@-rm $(libdir)/libiselpacks.a
	@-rm $(libdir)/libiselconsl.a
	@-rm $(bindir)/infosel++$(exesuf)
	@-rm $(bindir)/iseltest_api$(exesuf)
	@-rm $(bindir)/iseltest_ui$(exesuf)
	@-rm $(bindir)/iseltest_fe$(exesuf)
	@-rm $(outdir)/*.a
	@-rm $(outdir)/*.o
	@-rm $(includedir)/*.h
	@-rm $(docdir)/infosel-tutorial.*
	@-rm $(srcdir)/*.cpp
	@-rm $(srcdir)/*.cxx
	@-rm $(makedir)/*.sh
	@-rm $(makedir)/out/*.o
	@-rm $(testdir)/*.dat
	@-rm $(testdir)/*.ipp
	@-rm $(testdir)/*.sh
	@-rm $(workdir)/*.dat
	@-rm $(workdir)/*.ipp
	@-rm $(workdir)/*.sh
	@-rm $(workdir)/*.rep
	@echo no errors or up to date 1>&2
	@echo done target clean.

