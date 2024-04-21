CXX = g++
CXXFLAGS = -Wall -g

#For the CppUTest vesion installed manually
CPPUTEST_HOME =

ifneq ($(CPPUTEST_HOME),)
INCLUDEFLAGS += -I$(CPPUTEST_HOME)/include \
               -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h \
               -include $()/include/CppUTest/MemoryLeakDetectorMallocMacros.h
LIBFLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest
else
LIBFLAGS += -lCppUTest
#LIBFLAGS += -lCppUtestExt
endif

all:	core_test

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#CORE
CORE_DIR = ../../src/core
CORE_LIB = $(CORE_DIR)/core_lib.a
CORELIBFLAGS = -L$(CORE_DIR) -l:$(CORE_LIB)
INCLUDEFLAGS += -I$(CORE_DIR)
CXXFLAGS += $(INCLUDEFLAGS)

CORE_TESTS_OBJ = main_test.o \
                 game_field_test.o \
                 list_test.o \
                 message_test.o \
                 msg_buffer_test.o \
                 msg_delivery_control_test.o \
                 point_test.o \
                 random_test.o \
                 ship_test.o \
                 timer_test.o \
                 timeout_handler_test.o

core_test:	$(CORE_TESTS_OBJ) $(CORE_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBFLAGS)

../../src/core/core_lib.a:
	cd ../src/core/ ; $(MAKE)

clean:
	rm -f *.o core_test
