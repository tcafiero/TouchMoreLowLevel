TOOL_PREFIX ?= i686-pc-mingw32
GCC = $(TOOL_PREFIX)-gcc
GPP = $(TOOL_PREFIX)-g++
LD = $(TOOL_PREFIX)-gcc
KIND = -shared
OS_TARGET = win32
LIB_PREFIX =
LIB_SUFFIX = dll

SWIGINCLUDE ?= $(call FixPath,-I$(JDK_PATH)/include -I$(JDK_PATH)/include/$(OS_TARGET))
#SWIGINCLUDE ?= -I/usr/lib/ruby/1.8/i386-cygwin
SWIGFLAGS ?= -java
#SWIGFLAGS ?= -ruby

#JDK_PATH =
JDK_PATH = /cygdrive/c/Programmi/Java/jdk1.7.0_25

# switches for ruby or java wrapping
#OTHERSSWITCHES ?= -Wl,--enable-auto-image-base,--enable-auto-import,--export-all   -lruby  -lrt -ldl -lcrypt

LD_FLAGS = -Wl,--add-stdcall-alias
EXTRA_GPFLAGS = 

REMOVE = rm
QUIET=> /dev/null 2>&1 ; exit 0
FixPath = $1
#FixPath = $(subst /,\\,$(subst /cygwin/usr/bin,/cygwin/bin,$(subst /cygdrive/c,C:,$1)))

#INSTALLDIR ?= /usr/lib/ruby/site_ruby/1.8/i386-cygwin

