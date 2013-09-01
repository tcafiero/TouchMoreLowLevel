DSP_IMP_TEMPLATE = DSPimplementation
DSP_IMP_TEMPLATE_LIB = $(LIB_PREFIX)$(DSP_IMP_TEMPLATE).$(LIB_SUFFIX)
T1234 = $(wildcard ./sharedobjects/*.c)
SHRDOBJS = $(patsubst %.c,%.$(LIB_SUFFIX),$(T1234)) 

%.$(LIB_SUFFIX) : %.c
	$(GCC) $(GCOVFLAG) $(GCFLAGS) $(OTHERSSWITCHES) -c $< -o $(basename $<).o
	$(LD) $(LD_FLAGS) $(GCOVFLAGS) $(KIND) $(basename $<).o $(OTHERSSWITCHES) $(EXTRA_GPFLAGS) -o ./lib/$(LIB_PREFIX)$(notdir $@)
	@echo NOTICE
	@echo NOTICE
	@echo NOTICE remember to copy ./lib/$(LIB_PREFIX)$(notdir $@) into sysroot/usr/lib or sysroot/lib on the target system
	@echo NOTICE example for RaspberryPI cp ./lib/$(LIB_PREFIX)$(notdir $@) /usr/lib/
	@echo NOTICE
	@echo NOTICE
