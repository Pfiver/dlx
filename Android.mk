LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CFLAGS=-O3 --std=gnu99 -Wall
LOCAL_MODULE := suds
LOCAL_SRC_FILES := suds.c dlx.c
include $(BUILD_EXECUTABLE)
