CORE_DIR=../../core
CORE_LIB=$(CORE_DIR)/core_lib.a

CXX=g++
CXXFLAGS= -Wall -g -I$(CORE_DIR)
STATIC= -static

GAME_INTERFACE_OBJ= game_interface.o

all:	game_interface_lib

game_interface_lib:	$(GAME_INTERFACE_OBJ)
	$(AR) rcs game_interface_lib.a $(GAME_INTERFACE_OBJ)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o game_interface_lib.a
