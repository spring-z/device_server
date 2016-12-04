
CC=gcc

ROOT=.
PROJDIR=$(ROOT)/..

DEBUG=1

#通用组件
PUB_OBJDIR=$(ROOT)/public/obj
PUB_SRCDIR=$(ROOT)/public/src
PUB_INCDIR=$(ROOT)/public/inc


#库
LIB_INC=$(ROOT)/lib


#app
APP_SRCDIR=$(ROOT)/app
APP_OBJDIR=$(ROOT)/app/obj
APP_INCDIR=$(ROOT)/app

#main
TARGET1=device_server
TARGET1_MAIN=$(ROOT)/app/TcpServer.c


#通用组件编译
PUB_SRCS=$(wildcard $(PUB_SRCDIR)/*.c)
srcs=$(notdir $(PUB_SRCS))
PUB_OBJS=$(patsubst %.c,$(PUB_OBJDIR)/%.o,$(srcs))	

#APP文件编译
APP_SRCS=$(APP_SRCDIR)/ProtocolHandle.c $(APP_SRCDIR)/TcpServerWork.c
app_srcs=$(notdir $(APP_SRCS))
APP_OBJS=$(patsubst %.c,$(APP_OBJDIR)/%.o,$(app_srcs))	


#包含
INCS=-I$(PUB_INCDIR)
INCS+=-I$(APP_INCDIR)

#编译选项
ifeq ($(DEBUG),1)
	CFLAGS=-g -Wall -DDBUG
else
	CFLAGS=-Wall -DRELEASE
endif

OBJS=$(PUB_OBJS) $(APP_OBJS)


#库
#LIBS=-l/usr/lib
#LDFLAGS=-L$(PROJDIR)/lib

#链接
LINKFLAGS= -lpthread -lm -l mysqlclient

CFLAGS+=$(INCS)

.PHONY: all clean echo

all:$(TARGET1) 

$(TARGET1):$(OBJS) $(TARGET1_MAIN)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(TARGET1_MAIN) $(LINKFLAGS)
	

$(PUB_OBJS):$(PUB_SRCS)
	$(CC) $(CFLAGS) -c $(patsubst %.o,$(PUB_SRCDIR)/%.c,$(notdir $@)) -o $@

	
$(APP_OBJS):$(APP_SRCS)
	$(CC) $(CFLAGS) -c $(patsubst %.o,$(APP_SRCDIR)/%.c,$(notdir $@)) -o $@

	
echo:
	echo $(CC) -o $@ $(OBJS) $(CFLAGS) $(TARGET1_MAIN) $(LINKFLAGS)
	echo $(DEBUG)
	
clean:
	rm -f $(ROOT)/$(TARGET1) $(PUB_OBJDIR)/*.o $(APP_OBJDIR)/*.o

