CXX = clang++
CXXFLAGS = -Wall -Wshadow -std=c++1y -stdlib=libc++
EXENAME = pure-monad
CCMONAD = cc-monad
OBJS = monad.o help.o util.o filemap.o monad_shared.o svn.o
CCOBJS = ccmonad.o util.o monad_shared.o
IDFILE = ../.monadid
OPTIMIZE = off

ifeq ($(strip $(OPTIMIZE)),on)
CXXFLAGS += -O2 -DOPTIMIZE
else ifeq ($(strip $(OPTIMIZE)),off)
CXXFLAGS += -g -O0
else
$(warning Invalid value specified for OPTIMIZE. Should be on or off)
CXXFLAGS += -g -O0
endif

LIBS:=
IS_LIBRT:=$(shell echo "int main(){}" | clang++ -o /dev/null -x c++ - -lrt >/dev/null 2>/dev/null ; echo $$?)
IS_LIBPNG:=$(shell echo "int main(){}" | clang++ -o /dev/null -x c++ - -lpng >/dev/null 2>/dev/null ; echo $$?)
IS_LIBCXXABI:=$(shell echo "int main(){}" | clang++ -o /dev/null -x c++ - -lc++abi  >/dev/null 2>/dev/null ; echo $$?)
IS_LIBSVN:=$(shell echo "struct svn_version_t { int major, minor, patch; }; const struct svn_version_t *svn_subr_version(void); int main() { const struct svn_version_t *v = svn_subr_version(); return !(v->major == 1 && v->minor >= 6); }" | clang++ -o svn-version-check -x c - -lsvn_client-1 -lsvn_ra-1 -lsvn_subr-1 -lsvn_delta-1 -lapr-1 >/dev/null 2>/dev/null && ./svn-version-check ; echo $$? ; rm -f svn-version-check)
ifeq ($(IS_LIBRT),0)
LIBS+= -lrt
endif
ifeq ($(IS_LIBPNG),0)
LIBS+= -lpng 
endif
ifeq ($(IS_LIBCXXABI),0)
LIBS+= -lc++abi
endif
ifeq ($(IS_LIBSVN),0)
LIBS+= -lsvn_client-1 -lsvn_ra-1 -lsvn_subr-1 -lsvn_delta-1 -lapr-1
endif

.PHONY: monad
monad: $(EXENAME) $(CCMONAD) $(IDFILE)
.PHONY: all
all: monad

$(IDFILE): $(OBJS)
	echo -e '$(EXENAME) identification file\n'\
'This file is used for $(EXENAME) directory identification\n'\
'Built by' `whoami` 'on' `hostname`'\n'\
'Build Date:' `date`> $(IDFILE)

$(EXENAME): $(OBJS) $(IDFILE)
	$(CXX) $(LIBS) $(CXXFLAGS) $(OBJS) -o $@

$(CCMONAD): $(CCOBJS) $(IDFILE)
	$(CXX) $(LIBS) $(CXXFLAGS) $(CCOBJS) -o $@

help.o: help.cpp $(wildcard *.h) README.cgo README_config.cgo README_tests.cgo LICENSE.cgo quotes.cga

CONVERT_TXT_TO_CGO=sed -e 's/"/\\"/g' -e 's/^/    << "/' -e 's/$$/" << endl/'
CONVERT_TXT_TO_CGA=sed -e 's/"/\\"/g' -e 's/^/"/' -e 's/$$/",/'

%.cgo: %.txt
	$(CONVERT_TXT_TO_CGO) $(@:.cgo=.txt) > $@

%.cga: %.txt
	$(CONVERT_TXT_TO_CGA) $(@:.cga=.txt) > $@

%.o : %.cpp $(wildcard *.h)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@

svn.o: svn.h svn.cpp svn_libsvn.cpp
ifeq ($(IS_LIBSVN),0)
	$(CXX) $(CXXFLAGS) -I/usr/include/apr-1 -I/usr/include/apr-1.0 -I/usr/include/subversion-1 -Wno-deprecated-declarations -c svn_libsvn.cpp -o $@ || $(CXX) $(CXXFLAGS) -c svn.cpp -o $@
else
	$(CXX) $(CXXFLAGS) -c svn.cpp -o $@
endif

.PHONY: clean
clean:
	rm -f $(wildcard *.o) $(wildcard *.cgo) $(wildcard *.cga) $(EXENAME) $(CCMONAD) $(IDFILE)

