ROSS = /opt/CROSS_COMPILE/external-toolchain/bin/arm-linux-gnueabi-
CC = $(CROSS)gcc
CXX = $(CROSS)g++
AR = $(CROSS)ar
RANLIB = $(CROSS)ranlib
STRIP = $(CROSS)strip

#CFLAGS = -O3 -g -Wall -fPIC
CFLAGS = -O3 -g -fPIC
LDFLAGS += -shared

OBJS = serial.o
COURCE = serial.c

target = libserial.so

all:$(target)

$(target):$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	#$(CC) test.c -o test_demo -L. -lserial -lpthread
$(OBJS):$(SOURCE)
	$(CC) -I $(PWD) $(CFLAGS) -c $(COURCE)
.PHONY:clean
clean:
	rm *.o $(target)
