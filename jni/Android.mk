LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sample                                  #name-of-your-executable
LOCAL_SRC_FILES := argparser.c constants.c main.c search.c tools.c uefuncs.c
                      # source files
#LOCAL_CPPFLAGS := -std=gnu++0x -Wall -fPIE              # whatever g++ flags you like
#LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -fPIE -pie   # whatever ld flags you like

include $(BUILD_EXECUTABLE)    # <-- Use this to build an executable.