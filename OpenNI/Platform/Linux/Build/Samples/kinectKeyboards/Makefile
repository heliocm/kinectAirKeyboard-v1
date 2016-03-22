include ../../Common/CommonDefs.mak

BIN_DIR = ../../../Bin

INC_DIRS = \
	../../../../../Include \
	../../../../../Samples/kinectKeyboards

SRC_FILES = ../../../../../Samples/kinectKeyboards/*.cpp

CFLAGS += -std=c++11 

ifeq ("$(OSTYPE)","Darwin")
	LDFLAGS += -framework OpenGL -framework GLUT -lpthread
else
	USED_LIBS += glut GL pthread
endif

USED_LIBS += OpenNI

EXE_NAME = kinectKeyboards

include ../../Common/CommonCppMakefile

