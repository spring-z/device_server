
CC=gcc

ROOT=.
PROJDIR=$(ROOT)/..
OBJDIR=$(ROOT)/obj
SRCDIR=$(ROOT)/src

TARGET=UniTest
SRCS=$(wildcard $(SRCDIR)/*.c)
srcs=$(notdir $(SRCS))
OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(srcs))

ifeq ($(DEBUG),1)
	CFLAGS=-Wall -DDBUG
else
	CFLAGS=-Wall -DRELEASE
endif


INCS=-I$(PROJDIR)/lib/inc
INCS+=-I$(PROJDIR)/public/inc

LIBS=-lCommon -lPublic 
LDFLAGS=-L$(PROJDIR)/lib/obj
LDFLAGS+=-L$(PROJDIR)/public/obj


LINKFLAGS= -lpthread -lm

CFLAGS+=$(INCS)

.PHONY: all clean

all:$(TARGET) $(PROJDIR)/lib/obj/libCommon.a $(PROJDIR)/public/obj/libPublic.a 

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LINKFLAGS)
	#$(LDFLAGS) 
	#$(LIBS)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) -c $(patsubst %.o,$(SRCDIR)/%.c,$(notdir $@)) -o $@

clean:
	@echo "make" $(TARGET) "clean"
	rm -f $(ROOT)/$(TARGET) $(OBJDIR)/*.o

