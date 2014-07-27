#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include "waveManager.hh"
#include "organ.hh"
#include "../Network/Message.hpp"
#include <map>
using std::map;
using std::pair;


class Actor;
class Bang;
class Player;

class PlayState{
   static list<unsigned int> deletions;
   list<Actor*> actors;
   list<Bang*> bangs;
   list<Player*> players;
   long frameNum;
   WaveManager wm;
   int nbPlayers;
   unsigned kills;
   bool killsHaveChanged;

public:
   static PlayState* getInstance();
   static void killInstance();
   virtual ~PlayState();
   void tick();
   long getCurrentFrame() const { return frameNum;}
   void addBang(Bang* b) {bangs.push_back(b);}
   void addActor(Actor* a) {actors.push_back(a);}
   list<Actor*>& getActorList() {return actors;}
   list<Drawable> getChanges() const;
   list<unsigned int> getDeletions() const {
	  list<unsigned int> cp = deletions;
	  deletions.clear();
	  return cp;
   }
   void addDeletion(unsigned int uid) {deletions.push_back(uid); }
   void gameover();
   void giveMoney(int amount);
   list<Player*> getPlayers();
   Organ* getOrgan();
   Player* addPlayer(string classId, string name);
   unsigned int playerQuit(Player*);
   void addKill() {++kills; killsHaveChanged = true;}
   unsigned getKillsIfChanged() {if (killsHaveChanged) {killsHaveChanged = false; return kills;} return -1;}
   bool organIsAlive() {return getOrgan()->getHealth() > 0;}
   int getOrganHealthIfChanged();
   void addAIPlayer();

protected:
   Player* getPlayerFromChildID(uint8_t childID);
   unsigned int getScore();

private:
   PlayState() : frameNum(0), wm(), nbPlayers(0), kills(0), killsHaveChanged(true) {actors.push_back(new Organ());};
   static PlayState* instance_;
};

#endif /* _PLAYSTATE_H_ */
