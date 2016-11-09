#
#	Copyright (C) 2016 .
#


CC= gcc

RM= rm -f -r

#directory
PUB=../public
LIB_PRAVITE=../lib
OBJ_DIR=../obj
INC=../public

TARGET= TestEpoll

#link
LINKFLAGS=

#obj
OBJS_PUB= $(PUB)/LibEpoll.o $(PUB)/LibLoger.o $(PUB)/LibSocket.o $(PUB)/LibMsgQueue.o
OBJS_MAIN=  uni_epoll.o 


OBJS= $(OBJS_MAIN) $(OBJS_PUB) 


#taget
##########################################################
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -I $(INC) -o $(TARGET) -static $(LINKFLAGS)

.c.o : 
	$(CC) -c -I$(INC) $< -o $@

clean:
	$(RM) $(PUB)/*.o $(TARGET)
