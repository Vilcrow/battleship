CORE_DIR=../../core/
CORE_LIB=$(CORE_DIR)core_lib.a

CXX=g++
CXXFLAGS= -Wall -g -I$(CORE_DIR)
LIBFLAGS= -L$(CORE_DIR) -l:$(CORE_LIB)
STATIC= -static

CLIENT_OBJ= client_server.o

all:	client_lib

client_lib:	$(CLIENT_OBJ)
	$(AR) rcs client_lib.a $(CLIENT_OBJ)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o client_lib.a
