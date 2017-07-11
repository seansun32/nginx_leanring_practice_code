TARGET = prog
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

#OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
include $(PWD)/test_ngx_list_mak.mak
BINOBJS := $(DEPEND_FILES:%.c=%.o)

HEADERS = $(wildcard *.h)
INCLUDES += -I $(PWD)/../include/

%.o: %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

.PRECIOUS: $(TARGET) $(BINOBJS)

$(TARGET): $(BINOBJS)
	$(CC) $(BINOBJS) -Wall $(LIBS) -o $@

clean:
	-rm -f $(BINOBJS)
	-rm -f $(TARGET)
