#include "game.hh"
#include "../Processes/ProcessManager.hpp"
#include "../Client/Logger.hpp"
#include "../Network/ServerNetworkManager.hpp"
#include "player.hh"

Game* Game::instance_ = NULL;

void Game::killInstance() {
   delete instance_;
   instance_ = NULL;
}

Player* Game::getPlayerFromChildID(uint8_t childID){
   map<uint8_t, Player*>::iterator i;
   i = players.find(childID);
   if (i == players.end())
	  throw std::runtime_error("Game::getPlayerFromChildID : No such player: " + childID);
   return i->second;
}

void Game::processAddPlayerMessage(uint8_t childID){
   sf::Packet packet;
   ProcessManager* process = ProcessManager::getInstance();
   Logger* logger_ = Logger::getInstance();

   // 1) We add the player
   packet = process->receiveFromChild(childID);
   std::string playername;
   std::string className;
   sf::Uint8 playerClass;
   packet >> playername>>playerClass;
   if(playerClass==PLAYER_BUILDER)
	  className="builder";
   else if(playerClass==PLAYER_HEALER)
	  className="healer";
   else
	  className="fighter";
   
   sf::Uint32 pId;
   Player* tmp = PlayState::getInstance()->addPlayer(className, playername);
   players.insert(make_pair(childID, tmp));
   pId = tmp->getUid();
   logger_->log("GE: 	Player created");
   packet.Clear();
		 
   // 2) We send the player name to others
	process->sendToChild<Message>(childID,PLAYER_ID);
	packet <<pId;
	process->sendToChild(childID,packet);
	usleep(5);
   process->sendToChildren<Message>(PLAYER_NAME);
   packet << playername;
   process->sendToChildren(packet);
   packet.Clear();

   logger_->log("GE:	1 Player Ready");
  
}

void Game::processNOPMessage(uint8_t childID){
   ProcessManager* process = ProcessManager::getInstance();
   process->sendToChild<uint8_t>(childID,0);
}

void Game::processMouseClickMessage(uint8_t childID){
   sf::Packet packet;
   ProcessManager* process = ProcessManager::getInstance();
   packet = process->receiveFromChild(childID);
   sf::Uint8 id; sf::Int16 x; sf::Int16 y;
   packet>>id>>x>>y;
   Player* tmp = getPlayerFromChildID(childID);
   tmp->shoot(atan2(y - tmp->getY(), x - tmp->getX()));
   packet.Clear();
}

void Game::processChangeWeaponMessage(uint8_t childID){
   sf::Packet packet;
   ProcessManager* process = ProcessManager::getInstance();
   packet = process->receiveFromChild(childID);
   uint8_t weapon;
   packet >> weapon;
   if (getPlayerFromChildID(childID)->changeWeapon(weapon))
	  sendChangeWeaponMessage(childID, weapon);
}

void Game::processPlayerMoveMessage(uint8_t childID){
   ProcessManager* process = ProcessManager::getInstance();
   sf::Packet packet;
   packet = process->receiveFromChild(childID);
   sf::Uint8 dir;
   packet>>dir;
   getPlayerFromChildID(childID)->move(dir);
   packet.Clear();
}

void Game::processPlayerLeftMessage(uint8_t childID){
   ProcessManager* process = ProcessManager::getInstance();
   Logger* logger_ = Logger::getInstance();
   PlayState::getInstance()->playerQuit(getPlayerFromChildID(childID));
   players.erase(childID);
   process->childKilled(childID);  
   if(process->getNbChild()==0){
	  nbClient=0;
      logger_->log("GE:	No more child");
      gameOver = true;
   }
}

bool Game::processMessages(){
   ProcessManager* process = ProcessManager::getInstance();
   for(std::vector<uint8_t>::iterator it=process->firstChild();it != process->lastChild() && !gameOver; it++)
   {
	  usleep(100);
	  uint8_t childID = *it;
	  Message header = process->receiveFromChild<Message>(childID);
	  if(header==MSG_NOP)
		 processNOPMessage(childID);
	  else if(header==PLAYER_READY)
		 processAddPlayerMessage(childID);
	  else if(header==PLAYER_MOVE)
		 processPlayerMoveMessage(childID);
	  else if(header==MOUSE_CLICK)
		 processMouseClickMessage(childID);
	  else if(header==CHANGE_WEAPON)
		 processChangeWeaponMessage(childID);
	  else if(header==CLIENT_LEFT)
	  {
		 processPlayerLeftMessage(childID);
		 it=process->firstChild(); 
	  }
	  
   }
   return !gameOver;
}

void Game::tick(){
   if (currentState)
	  currentState->tick();
   sendPlayerChanges();
}

void Game::sendPlayerChanges(){
   int oHp = getOrganHealthIfChanged(); // todo: fugly, dup code: make somekind of send method
   if (oHp != -1){
	  ProcessManager* process = ProcessManager::getInstance();
	  sf::Packet packet;
	  process->sendToChildren<Message>(ORGAN_HP);
	  packet << (sf::Int16)oHp;
	  process->sendToChildren(packet);
   }
   int kills = PlayState::getInstance()->getKillsIfChanged();
   if (kills != -1){
	  ProcessManager* process = ProcessManager::getInstance();
	  sf::Packet packet;
	  process->sendToChildren<Message>(UPDATE_KILLS);
	  packet << (sf::Int16)kills;
	  process->sendToChildren(packet);   
   }
   for (map<uint8_t, Player*>::iterator i = players.begin(); i != players.end(); ++i){
	  int pHp = i->second->getHealthIfChanged();
	  uint8_t childId = i->first;
	  if (pHp != -1){
		 ProcessManager* process = ProcessManager::getInstance();
		 sf::Packet packet;
		 process->sendToChild<Message>(childId, PLAYER_HP); 
		 packet << (sf::Int16)pHp;
		 process->sendToChild(childId, packet);
		 packet.Clear();
	  }
	  int money = i->second->getMoneyIfChanged();
	  if (money != -1){
		 ProcessManager* process = ProcessManager::getInstance();
		 sf::Packet packet;
		 process->sendToChild<Message>(childId, PLAYER_MONEY); 
		 packet << (sf::Int16)money;
		 process->sendToChild(childId, packet);
	  }
   }
}

void Game::warnPlayersOnNewWave(list<string> &startingPositions){
   ProcessManager* process = ProcessManager::getInstance();
   sf::Packet packet;
   process->sendToChildren<Message>(NEW_WAVE);
   packet << (sf::Uint8)startingPositions.size();
	
   for(std::list<std::string>::iterator i = startingPositions.begin(); i != startingPositions.end(); ++i)
	  packet << (*i);
	
   process->sendToChildren(packet);
}

void Game::sendScoreToClients(unsigned int score) {
   ProcessManager* process = ProcessManager::getInstance();
   sf::Packet packet;
   process->sendToChildren<Message>(SCORE);
   packet << (sf::Uint16)score;
	
   process->sendToChildren(packet);
}

void Game::sendChangeWeaponMessage(uint8_t childID, uint8_t weapon) {
   ProcessManager* process = ProcessManager::getInstance();
   sf::Packet packet;
   packet << weapon;
   process->sendToChild<Message>(childID, CHANGE_WEAPON);
   process->sendToChild(childID, packet);
}

list<Drawable> Game::getChanges() const{
   return currentState->getChanges();
}

list<unsigned int> Game::getDeletions() const{
   return currentState->getDeletions();
}
