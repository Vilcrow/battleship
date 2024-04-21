CORE_DIR=../core/
CORE_LIB=$(CORE_DIR)core_lib.a
BIN_DIR=../../bin/

CXX=g++
CXXFLAGS= -Wall -g -I$(CORE_DIR)
LIBFLAGS= -L$(CORE_DIR) -l:$(CORE_LIB)
STATIC= -static

SERVER_OBJ= bs_server.o client_array.o client_data.o \
            game_session.o preparation_timeout.o main.o 

all:	bs_server

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bs_server:	$(SERVER_OBJ) $(CORE_LIB)
	$(CXX) $(CXXFLAGS) $(STATIC) $^  -o $@ $(LIBFLAGS)
	mv bs_server $(BIN_DIR)

../core/core_lib.a:
	cd ../core/ ; $(MAKE)

clean:
	rm -f *.o bs_server
