#include "playstate.hh"
#include "bang.hh"
#include "game.hh"
#include "player.hh"
#include "weapon.hh"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "aiPlayer.hh"

list<unsigned int> PlayState::deletions;

PlayState* PlayState::instance_ = NULL;

PlayState* PlayState::getInstance(){
   if(!instance_)
	  instance_ = new PlayState();
   return instance_;
}

void PlayState::killInstance(){
   delete instance_;
   instance_ = NULL;
}


PlayState::~PlayState(){
   for (list<Actor*>::iterator i = actors.begin(); i != actors.end(); ++i)
	  delete *i;
   actors.clear();
   for (list<Bang*>::iterator i = bangs.begin(); i != bangs.end(); ++i)
	  delete *i;
   bangs.clear();
   deletions.clear();
}


void PlayState::tick(){
   if (frameNum == 0)
	  for (int i = 0; i < MAX_PLAYERS - nbPlayers; ++i)
		 addAIPlayer();
   ++frameNum;
   for (list<Actor*>::iterator i = actors.begin(); i != actors.end();){
	  if (!(*i)->tick()){
		 delete *i;
		 i = actors.erase(i);
	  }
	  else
		 ++i;
   }
    
   for (list<Bang*>::iterator i = bangs.begin(); // todo: merge above
		i != bangs.end();){
	  if (!(*i)->tick()){
		 delete *i;
		 i = bangs.erase(i);
	  }
	  else
		 ++i;
   }
   if (organIsAlive())
	  wm.tick(frameNum);
}

void PlayState::gameover(){
   list<Player*> players = getPlayers();
   for (list<Player*>::iterator i = players.begin(); i != players.end(); ++i)
	  (*i)->hurt((*i)->getHealth());
	
   Game::getInstance()->sendScoreToClients(getScore());
}

unsigned int PlayState::getScore() {
	unsigned int score = 0;
	
	score += frameNum / TICKS_PER_SECONDS;
	score += kills*2;
	
	return score;
}

void PlayState::giveMoney(int amount){
   list<Player*> players = getPlayers();
   for (list<Player*>::iterator i = players.begin(); i != players.end(); ++i)
	  (*i)->giveMoney(amount);
}

list<Player*> PlayState::getPlayers(){
//    list<Player*> tmp;
//    list<Actor*>::iterator i = ++(actors.begin());
//    for (int j = 0; j < nbPlayers; ++i, ++j){
// 	  Player* p = dynamic_cast<Player*>(*i);
// 	  if (!p)
// 		 throw std::runtime_error("ps::getPlayers: screwed up actors list");
// 	  tmp.push_back(p);
//    }
   return players;
}

list<Drawable> PlayState::getChanges() const{
   list<Drawable> dl;
   for (list<Actor*>::const_iterator i = actors.begin(); i != actors.end(); ++i)
	  if ((*i)->isVisible() && (*i)->hasBeenModified())
		 dl.push_back((*i)->getDrawable()); // todo: merge
   for (list<Bang*>::const_iterator i = bangs.begin(); i != bangs.end(); ++i){
	  if ((*i)->isVisible() && (*i)->hasBeenModified())
		 dl.push_back((*i)->getDrawable());
   }
   return dl;
}

Player* PlayState::addPlayer(string classId, string name){
   ++nbPlayers;
   Player *tmp = new Player(name, classId);
   actors.push_back(tmp);
   players.push_back(tmp);
   return tmp;
}

void PlayState::addAIPlayer(){ // todo: make a clean version
   string c = "fighter";
   if (rand()%4)
	  c = "healer";
   Player* tmp = new AIPlayer(c);
   players.push_back(tmp);
   addActor(tmp);
}

unsigned int PlayState::playerQuit(Player* tmp){
   unsigned int uid = tmp->getUid();
   --nbPlayers;
   actors.remove(tmp);
   players.remove(tmp);
   delete tmp;
   return uid;
}

int PlayState::getOrganHealthIfChanged(){
   return getOrgan()->getHealthIfChanged();
}

Organ* PlayState::getOrgan(){
   Organ* tmp = dynamic_cast<Organ*>(actors.front());
   if (!tmp)
	  throw std::runtime_error("PS::getOrgan: Organ not at front of list");
   return tmp;
}
