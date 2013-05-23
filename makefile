CC=g++
INC+=-I./
LIB+=-lpthread
CFLAGS+=-g -Wall -pipe

OBJ=cbs_buf.o cbs_device.o cbs_target.o cbs_initiator.o cbs_timer.o main.o

mytest:$(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)
%.o:%.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
