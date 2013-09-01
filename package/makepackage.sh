#!/bin/bash
rm -v -f ./DSPwrapper.7z
/cygdrive/c/HolisticSystems/tools/7za.exe a -t7z ./DSPwrapper.7z ../src/wrappers/*.java ../src/wrappers/*.c
