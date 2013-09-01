TOOL_PREFIX ?= /cygdrive/c/HolisticSystems/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi
#TOOL_PREFIX ?= /cygdrive/c/Programmi/CodeSourcery/Sourcery_CodeBench_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi
GCC = $(TOOL_PREFIX)-gcc
GPP = $(TOOL_PREFIX)-g++
LD = $(TOOL_PREFIX)-gcc
KIND = -shared
OS_TARGET = linux
LIB_PREFIX = lib
LIB_SUFFIX = so


SWIGINCLUDE ?= $(call FixPath,-I$(JDK_PATH)/include -I$(JDK_PATH)/include/$(OS_TARGET))
#SWIGINCLUDE ?= -I/usr/lib/ruby/1.8/i386-cygwin
SWIGFLAGS ?= -java
#SWIGFLAGS ?= -ruby

JDK_PATH ?= .
#JDK_PATH ?= /cygdrive/c/Programmi/Java/jdk1.7.0_25

# switches for ruby or java wrapping
#OTHERSSWITCHES ?= -Wl,--enable-auto-image-base,--enable-auto-import,--export-all   -lruby  -lrt -ldl -lcrypt

#EXTRA_GPFLAGS = -mfloat-abi=hard -static-libgcc -lc -lgcc
#EXTRA_GPFLAGS = -mfloat-abi=hard -static-libgcc -lc -lgcc -march=armv7-a -mfpu=neon
EXTRA_GPFLAGS = -mfloat-abi=soft -static-libgcc -lc -lgcc
LD_FLAGS = 

REMOVE = rm
QUIET=> /dev/null 2>&1 ; exit 0
#FixPath = $1
#FixPath = $(subst /,\\,$(subst /cygwin/usr/bin,/cygwin/bin,$(subst /cygdrive/c,C:,$1)))
FixPath = $(subst /cygdrive/c,C:,$1)

#INSTALLDIR ?= /usr/lib/ruby/site_ruby/1.8/i386-cygwin

