CXX=g++
CXXFLAGS= -Wall -g -c
STATIC= -static

SERVER_OBJ= game_field.o message.o msg_buffer.o \
            msg_delivery_control.o point.o random.o \
            ship.o timer.o \
            timeout_handler.o tools/write_log.o

all:	core_lib

core_lib:	$(SERVER_OBJ)
	$(AR) rcs core_lib.a $(SERVER_OBJ)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.a
