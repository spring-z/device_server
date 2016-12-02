
CC=gcc

ROOT=.
PROJDIR=$(ROOT)/..
OBJDIR=$(ROOT)/obj
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/inc

#目标文件1
TARGET1=UniTest
TARGET1_MAIN=test.c

#目标文件2
TARGET2=UniTest2
TARGET2_MAIN=test2.c

#获取src目录下所有.c文件
SRCS=$(wildcard $(SRCDIR)/*.c)
#去除SRCS目录信息
srcs=$(notdir $(SRCS))
#将所有.c文件对应为同名obj目录下.o目标文件
OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(srcs))	


#包含
INCS=-I$(INCDIR)
#INCS+=-I$(PROJDIR)/lib/inc

#编译选项
ifeq ($(DEBUG),1)
	CFLAGS=-Wall -DDBUG
else
	CFLAGS=-Wall -DRELEASE
endif

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
	
$(TARGET2):$(OBJS) $(TARGET2_MAIN)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(TARGET2_MAIN) $(LINKFLAGS)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) -c $(patsubst %.o,$(SRCDIR)/%.c,$(notdir $@)) -o $@

	
echo:
	echo $(OBJS)
	
clean:
	rm -f $(ROOT)/$(TARGET1) $(OBJDIR)/*.o

