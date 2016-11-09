#
#	Copyright (C) 2016 .
#


CC= gcc

RM= rm -f -r

#directory
ROOT=.

PUB=$(ROOT)/public
LIB_PRAVITE=$(ROOT)/lib
OBJ_DIR=$(ROOT)/obj
INC=$(ROOT)/public

TARGET= TestEpoll

#link
LINKFLAGS=

#src 
SRC = $(wildcard ${PUB}/*.c) 
#SRC = $(wildcard *.c) $(wildcard *.c)

#obj
OBJ = $(patsubst %.c,${OBJ_DIR}/%.o,$(notdir ${SRC}))


CFLAGS = -g -Wall


#taget
##########################################################
all: $(TARGET)
${TARGET}: ${OBJ}
	$(CC) $(OBJ) -I $(INC) -o $(TARGET) -static $(LINKFLAGS)

{OBJ_DIR}/%.o:${PUB}/%.c 
	$(CC) -c $(CFLAGS) -I$(INC) $< -o $@

clean:
	$(RM) $(PUB)/*.o $(TARGET)
