#ifndef _GAME_H_
#define _GAME_H_

#include <list>
using std::list;

#include "playstate.hh"
#include "player.hh"
#include "../Network/Message.hpp"


class Game {
protected:
   PlayState* currentState;
   map<uint8_t, Player*> players;
   bool gameOver;
   Player* getPlayerFromChildID(uint8_t childID);
   void sendPlayerChanges();
   void processAddPlayerMessage(uint8_t);
   void processNOPMessage(uint8_t);
   void processMouseClickMessage(uint8_t);
   void processChangeWeaponMessage(uint8_t);
   void processPlayerMoveMessage(uint8_t);
   void processPlayerLeftMessage(uint8_t);
   int getOrganHealthIfChanged() {return PlayState::getInstance()->getOrganHealthIfChanged();}
   int getPlayerHealthIfChanged(uint8_t childID) {return getPlayerFromChildID(childID)->getHealthIfChanged();}
   void sendChangeWeaponMessage(uint8_t, uint8_t);
public:
   void warnPlayersOnNewWave(list<string> &startingPositions);
   void sendScoreToClients(unsigned int score);
   
   static Game* getInstance() {
	  if(instance_ == NULL)
		 instance_ = new Game();
	  return instance_;
   }
   
   static void killInstance();
   ~Game(){
	  players.clear();
	  PlayState::killInstance();
   }
   void tick();
   list<Drawable> getChanges() const;
   list<unsigned int> getDeletions() const;
   unsigned int addPlayer(uint8_t childID, const int classID, std::string name);

   bool processMessages();
 
   bool organIsAlive() {return PlayState::getInstance()->organIsAlive();}
   int getNbClient() const {return nbClient;} 
   void addClient(){++nbClient;}
   void rmClient(){--nbClient;}
   
private:
  Game() : currentState(PlayState::getInstance()), gameOver(false),nbClient(0){
  }
   static Game* instance_;
   int nbClient;
   
};

#endif /* _GAME_H_ */
