T432 = $(wildcard ./src/wrappers/*.i)
JAVA_FILES = ./src/wrappers/*.java
WRAPPEROBJS = $(patsubst %.i,%_wrap.o,$(T432))
SHAREDINCLUDE ?= $(call FixPath,-I./sharedobjects)
WRAPPERLIB = $(LIB_PREFIX)$(ProjName).$(LIB_SUFFIX)



%_wrap.c	: %.i
	swig $(SWIGFLAGS) -o $@ $<
ifeq "$(SWIGFLAGS)" "-java"
	javac $(JAVA_FILES)
endif

%_wrap.o	: %_wrap.c
ifeq "$(OS_TARGET)" "linux"
	sed 's/__CYGWIN__/__WIN32__/' $<  >tmp
	cp -f tmp $<
	rm -f tmp
endif
	cp -f $(basename $<).c $(basename $<)save.c
	$(GCC) $(GCOVFLAG) $(DBG) -Wall $(OPTIMIZATION) $(GCFLAGS) -pipe -fno-strict-aliasing $(TESTFLAG) $(SWIGINCLUDE) $(SHAREDINCLUDE) -c $< -o $@
	