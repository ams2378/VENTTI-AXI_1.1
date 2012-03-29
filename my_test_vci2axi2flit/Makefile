#
# Makefile
# Author: Michele Petracca
# Affiliation: Columbia University
# System-on-Chip Design, Spring 2012
#

SYSTEMC := /usr/local/systemc-2.2
TLM := /home/geo/tools/TLM-2009-07-15
TARGET_ARCH := linux
WAVE_VIEWER := simvision
CC := g++

LIBS   := -lsystemc
LIBDIR = -L. -L$(SYSTEMC)/lib-$(TARGET_ARCH)
INCDIR = -I. -I$(SYSTEMC)/include -I$(TLM)/include/tlm

TARGET := sc_main
VPATH := ../src:../tb
#SRCS := slave_device.cpp dummy_slave.cpp master_device.cpp network_base.cpp dummy_master.cpp sc_main.cpp
SRCS := slave_device.cpp dummy_slave.cpp master_device.cpp dummy_master.cpp sc_main.cpp
OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .cpp .cc .o
$(TARGET): $(OBJS)
	$(CC) ${INCDIR} ${LIBDIR} -o $@ $(OBJS) ${LIBS} -DUSE_SYSTEMC_TLM

.PHONY: clean run wave

.cpp.o:
	$(CC) -Wall ${INCDIR} -c $< -DUSE_SYSTEMC_TLM

clean:
	rm -f *.o $(TARGET) *.dsn *.trn *.vcd
	if [ -d .simvision ]; then rm -r .simvision; fi

run: $(TARGET)
	./$(TARGET)

wave:
	$(WAVE_VIEWER) $(TARGET).vcd
