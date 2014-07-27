###############################################################
# 					MakeFile Organe Defence		    		  #
#-------------------------------------------------------------#
#	Procédure à suivre lors de l'ajout d'un nouveau module 	  #
#-------------------------------------------------------------#
#	1) Vérifier que votre module ne se trouve pas dans un	  #
#	   de ces dossiers : 									  #
#	   -Network												  #
#	   -Process												  #
#	   -Client												  #
#	   -GameEngine											  #
#															  #
#	   Si c'est le cas, vous ne devez normalement RIEN FAIRE  #
#	   L'ajout de votre module se fera automatiquement.		  #
#															  #
# 	   Si Malgrés tout, vous rencontrer un problème, passez   #
#	   à l'étape 2 ci dessous ( ou demandez à Balor)	  	  #
#															  #
# 	   Si c'est un test unitaire que vous voulez ajouter,	  #
#	   demandez à Balor.			  	 					  #
#															  #
#	2) ajouter le chemin complet vers les cpp ci-dessous 	  #
#	   après le = de MODULE_SERVER et MODULE_CLIENT			  #
#	   suivant que votre nouveau module est pour le serveur   #
#	   le client. Si votre module concerne les deux, 		  #
#	   ajoutez son cpp dans les deux variables.				  #
#															  #
#	3) Prévenir Balor, car cette méthode n'est que temporaire #
#	   mais devrait fonctionner parfaitement et permettre	  #
#	   à votre module de s'intégrer au reste du code		  #
###############################################################

MODULE_SERVER=
MODULE_CLIENT=

###############################################################
# 						NE PAS TOUCHER  					  #
###############################################################

UNAME := $(shell uname)

# LDFLAGS= -lSDL -lSDL_image -lSDLmain -lSDL_ttf -lsocket
LDFLAGS= -lSDL_image -lSDL_ttf

DEBUG = no

ifeq ($(DEBUG), yes)
	CFLAGS= -W -Wall -ansi -pedantic  -g
else
	CFLAGS= -W -Wall -ansi -pedantic -O3
endif

ifeq ($(UNAME), Linux)
	LDFLAGS_S=
endif
ifeq ($(UNAME), SunOS)
	LDFLAGS_S += -lsocket
	LDFLAGS += -lsocket -lnsl
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS= -framework SDL -framework SDL_image -framework SDL_ttf -framework Cocoa -arch i386
	LDFLAGS_S=-arch i386
	CFLAGS += -arch i386
else
	SDL_CFLAGS := $(shell sdl-config --cflags)
	SDL_LDFLAGS := $(shell sdl-config --libs)

	FT_CFLAGS := $(shell freetype-config --cflags)
	FT_LDFLAGS := $(shell freetype-config --libs)
endif

CC=g++

# Inits
S_INIT=inits/common_inits.o inits/server_inits.o
C_INIT=inits/common_inits.o inits/client_inits.o
# Network modules
N_DIR=Network/
NET_COMMON= $(N_DIR)NetworkManager.cpp $(N_DIR)Packet.cpp
NET_S= $(N_DIR)ServerNetworkManager.cpp
NET_C= $(N_DIR)ClientNetworkManager.cpp
O_NET_COMMON=$(NET_COMMON:.cpp=.o) 
O_NET_C=$(NET_C:.cpp=.o) 
O_NET_S= $(NET_S:.cpp=.o) 
#Processes module
SRCP= $(wildcard Processes/*.cpp)
OBJP= $(SRCP:.cpp=.o)
# client modules
SRCG=$(wildcard Client/*.cpp )
HG = Client/Entity.hpp  Client/ImageLoader.hpp
OBJG= $(SRCG:.cpp=.o)
# GameEngine modules
SRCGE=$(wildcard GameEngine/*.cc )
OBJGE= $(SRCGE:.cc=.o)
#Tests
TESTDIR=tests/

S_OBJ=$(MODULE_SERVER:.cpp=.o) $(O_NET_COMMON) $(O_NET_S) $(S_INIT) servermain.o $(OBJP) Client/Timer.o  Client/Logger.o $(OBJGE) 
C_OBJ=$(MODULE_CLIENT:.cpp=.o) $(O_NET_COMMON) $(O_NET_C) $(C_INIT) client.o $(OBJG) 

all: organe_defense server
ifeq ($(DEBUG),yes)
	@echo "Génération en mode debug"
else
	@echo "Génération en mode release"
endif


###############################################################
# label, définissant le nom et les modules à linker ensemble  #
#					Ici client et server					  #
###############################################################
ifeq ($(UNAME), Darwin)
organe_defense: $(C_OBJ) SDLmain/SDLMain.o
	$(CC) -o $@ $^ $(LDFLAGS)

SDLmain.o: SDLMain.h SDLMain.m
	gcc -lobjc -o SDLMain.o SDLmain/SDLMain.m -arch i386 

else
organe_defense: $(C_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(SDL_LDFLAGS) $(FT_LDFLAGS)
endif

server: $(S_OBJ)
	 $(CC) -o $@ $^ $(LDFLAGS_S)

###############################################################
# label, définissant le nom et les modules à linker ensemble  #
#					   Ici les tests						  #
###############################################################
TFLAG=-lboost_unit_test_framework
SRCT=$(wildcard tests/*.cc )
tests: Tprocess TGameEngine
	@echo "Test Process and Network"
	./Tprocess
	@echo "Test GameEngine"
	./TGameEngine

Tprocess: $(TESTDIR)testProcessManager.o $(OBJP) $(O_NET_COMMON) $(O_NET_S) $(O_NET_C)
	$(CC) -o  $@ $^ $(LDFLAGS_S)
	 
TGameEngine: $(SRCT) $(OBJGE) $(OBJP) $(O_NET_COMMON) Client/Logger.o
	$(CC) -o  $@ $^ $(LDFLAGS_S) $(TFLAG)

###############################################################
#	Régles générales de compilation, défini les dépendances   #
#	de base et la commande a effectué pour la compilation	  #
###############################################################	 
#ifeq ($(UNAME), Darwin)
%.o: %.cpp %.hpp
	$(CC) -o $@ -c $< $(CFLAGS)
#else
#%.o: %.cpp %.hpp
#	$(CC) -o $@ -c $< $(CFLAGS) $(SDL_CFLAGS) $(FT_CFLAGS)
#endif

%.o: %.cc %.hh GameEngine/consts.hh GameEngine/DrawableType.hh
	$(CC) -o $@ -c $< $(CFLAGS)
###############################################################
#	Dépendances des modules, cas qui ne rentre pas dans la    #
#						règle générale						  #
###############################################################	 
Client/SceneManager.o: Client/SceneManager.hpp Client/SceneManager.cpp Client/Entity.hpp
Network/NetworkManager.o: Network/NetworkManager.hpp Network/NetworkManager.cpp Network/Message.hpp
Network/ClientManager.o: Network/NetworkManager.o Network/ClientManager.cpp Network/ClientManager.hpp
Network/ServerManager.o: Network/NetworkManager.o Network/ServerManager.cpp Network/ServerManager.hpp
servermain.o :servermain.cpp $(SRC) $(SRCP) $(SRCGE) $(MODULE_SERVER) inits/server_inits.cpp inits/server_inits.hpp GameEngine/DrawableType.hh
	$(CC) -o $@ -c servermain.cpp $(CFLAGS)
ifeq ($(UNAME), Darwin)
client.o: client.cpp $(SRC) $(SRCG) $(HG) $(MODULE_CLIENT) inits/client_inits.cpp inits/client_inits.hpp GameEngine/DrawableType.hh
	$(CC) -o $@ -c client.cpp $(CFLAGS)
else
client.o: client.cpp $(SRC) $(SRCG) $(HG) $(MODULE_CLIENT) inits/client_inits.cpp inits/client_inits.hpp GameEngine/DrawableType.hh
	$(CC) -o $@ -c client.cpp $(CFLAGS) $(SDL_CFLAGS) $(FT_CFLAGS)
endif

###############################################################
#	Label de clean, suppresion des objets compilés		      #
#	Label cleana supprime en plus tous les exécutables		  #
###############################################################	

.PHONY: clean cleana

clean: 
	rm -rf *.o
	rm -rf */*.o
	clear

cleana: clean
	rm -f server
	rm -f organe_defense
	rm -f $(TESTDIR)Tprocess
	rm -f $(TESTDIR)T2d
	rm -f Tprocess
	rm -f TGameEngine
	clear
