DEBUG = yes

CC=g++
ifeq ($(DEBUG),yes)
	CFLAGS= -W -Wall -ansi -pedantic -O -g
	#CFLAGS= -O -g
	LDFLAGS= -lSDL -lSDLmain -lSDL_image
else
	CFLAGS= -O
	#CFLAGS= -W -Wall -ansi -pedantic -O
	LDFLAGS= -lSDL -lSDLmain -lSDL_image
endif

# Network modules
SRC= $(wildcard Network/*.cpp)
OBJ= $(SRC:.cpp=.o)
#Processes module
SRCP= $(wildcard Processes/*.cpp)
OBJP= $(SRCP:.cpp=.o)
# client modules
SRCG=$(wildcard Client/*.cpp )
HG = $(wildcard Client/*.h )
OBJG= $(SRCG:.cpp=.o)
# GameEngine modules
SRCGE=$(wildcard GameEngine/*.cc )
OBJGE= $(SRCGE:.cc=.o)
#Tests
TESTDIR=tests/


all: client server
ifeq ($(DEBUG),yes)
	@echo "Génération en mode debug"
else
	@echo "Génération en mode release"
endif

tests: Tprocess T2d
	@echo "Test process"
	$(TESTDIR)/Tprocess
	@echo "Test TwoD"
	$(TESTDIR)/T2d

client: $(OBJ) client.o $(OBJG)
	$(CC) -o $@ $^ $(LDFLAGS)

server: $(OBJ) servermain.o $(OBJP) Client/Timer.o $(OBJGE)
	 $(CC) -o $@ $^ $(LDFLAGS)

Tprocess: $(TESTDIR)testProcessManager.o $(OBJP)	
	$(CC) -o  $(TESTDIR)$@ $^ $(LDFLAGS)
	 
T2d: $(TESTDIR)test2dObj.cc $(OBJGE)
	$(CC) -o  $(TESTDIR)$@ $^ $(LDFLAGS)
################################################	 
%.o: %.cpp %.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cc %.hh
	$(CC) -o $@ -c $< $(CFLAGS) 

################################################
servermain.o : $(SRC) $(SRCP) $(SRCGE) constants.hpp
client.o: $(SRC) $(SRCG) constants.hpp $(HG)
################################################

clean: 
	rm -rf *.o
	rm -rf */*.o
	clear
cleana: clean
	rm -f server
	rm -f client
	rm -f $(TESTDIR)Tprocess
	rm -f $(TESTDIR)T2d
	clear
