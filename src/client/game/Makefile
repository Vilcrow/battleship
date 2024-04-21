CORE_DIR=../../core
CORE_LIB=$(CORE_DIR)/core_lib.a
INTERFACE_DIR=../interface
INTERFACE_LIB=$(INTERFACE_DIR)/game_interface_lib.a

NCURSES_DIR=../../libs/ncurses-6.4
NCURSES_LIB_DIR=$(NCURSES_DIR)/lib
NCURSES_LIB=$(NCURSES_LIB_DIR)/libncurses.a

NCURSES_INTERFACE_DIR=../ncurses
NCURSES_INTERFACE_LIB=$(NCURSES_INTERFACE_DIR)/ncurses_interface_lib.a

CLIENT_DIR=../server
CLIENT_LIB=$(CLIENT_DIR)/client_lib.a
BIN_DIR=../../../bin

CXX=g++
CXXFLAGS= -Wall -g -I$(CORE_DIR) -I$(CLIENT_DIR) \
          -I$(NCURSES_DIR)/include \
          -I$(NCURSES_INTERFACE_DIR) -I$(INTERFACE_DIR)
LIBFLAGS= -L$(CORE_DIR) -l:$(CORE_LIB) \
          -L$(CLIENT_DIR) -l:$(CLIENT_LIB) \
          -L$(INTERFACE_DIR) -l:$(INTERFACE_LIB) \
          -L$(NCURSES_INTERFACE_DIR) -l:$(NCURSES_INTERFACE_LIB) \
          -L$(NCURSES_LIB_DIR) -l:$(NCURSES_LIB)

STATIC= -static

GAME_OBJ= main.o bs_game.o

all:	bs_game

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bs_game:	$(GAME_OBJ) $(CORE_LIB) $(INTERFACE_LIB) $(NCURSES_INTERFACE_LIB) $(CLIENT_LIB) $(NCURSES_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBFLAGS)
	mv bs_game $(BIN_DIR)
	cp -r $(NCURSES_INTERFACE_DIR)/files $(BIN_DIR)

../../core/core_lib.a:
	cd $(CORE_DIR) ; $(MAKE)

../server/client_lib.a:
	cd $(CLIENT_DIR) ; $(MAKE)

../interface/game_interface_lib.a:
	cd $(INTERFACE_DIR) ; $(MAKE)

$(NCURSES_LIB):
	cd $(NCURSES_DIR) ; ./configure ; $(MAKE)

../ncurses/ncurses_interface_lib.a:
	cd $(NCURSES_INTERFACE_DIR) ; $(MAKE)

clean:
	rm -f *.o bs_game
