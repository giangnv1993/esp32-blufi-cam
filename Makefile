#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

#PROJECT_NAME := face_recognition_solution
#
#SOLUTION_PATH ?= $(abspath $(shell pwd))/../../..
#
#include $(SOLUTION_PATH)/components/component_conf.mk
#include $(IDF_PATH)/make/project.mk

PROJECT_NAME := blufi_demo

COMPONENT_ADD_INCLUDEDIRS := components/include

include $(IDF_PATH)/make/project.mk