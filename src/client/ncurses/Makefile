CORE_DIR=../../core
GAME_INTERFACE_DIR=../interface
GAME_INTERFACE_LIB=$(GAME_INTERFACE_DIR)/game_interface_lib.a
NCURSES_DIR=../../libs/ncurses-6.4
NCURSES_LIB_DIR=$(NCURSES_DIR)/lib
NCURSES_LIB=$(NCURSES_LIB_DIR)/libncurses.a

CXX=g++
CXXFLAGS= -Wall -g -I$(GAME_INTERFACE_DIR) -I$(CORE_DIR) -I$(NCURSES_DIR)/include
STATIC= -static

NCURSES_INTERFACE_OBJ= ncurses_interface.o cursor.o

all:	ncurses_interface_lib

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

ncurses_interface_lib:	$(NCURSES_INTERFACE_OBJ)
	$(AR) rcs ncurses_interface_lib.a $(NCURSES_INTERFACE_OBJ)

$(NCURSES_LIB):
	cd $(NCURSES_DIR) ; ./configure ; $(MAKE)

clean:
	rm -f *.o ncurses_interface_lib.a
