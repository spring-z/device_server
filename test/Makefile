#
#	Copyright (C) 2016 .
#


CC= g++

RM= rm -f -r

#directory

PUB=../public
LIB_PRAVITE=../lib/
OBJ_DIR=../obj

TARGET= TestEpoll
OBJ_MAIN= 



OBJS_PUB= $(PUB)/LibEpoll.o $(PUB)/LibLogger.o $(PUB)/LibSocket.o
OBJS_MAIN=  uni_main.o 


OBJS= $(OBJS_MAIN) $(OBJS_PUB) 




#taget
##########################################################
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET) -static $(LINKFLAGS)



clean:
	$(RM) $(OBJ_DIR)/*.o $(TARGET)
