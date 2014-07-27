#include "player.hh"
#include "consts.hh"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
using std::max;
#include <utility>
using std::make_pair;
#include "weapon.hh"
#include "playstate.hh"
#include <climits>
#include <utility>
using std::make_pair;
#include "util.hh"
#include <algorithm>
#include "game.hh"


void PlayerClass::load(string name){
   string fn = DATA_DIR + name + ".class";
   ifstream fs(fn.c_str());
   if (!fs)
	  throw std::runtime_error("no such class: " + name);
   string prop;
   while (fs >> prop){
	  if (prop == "img:")
		 fs >> img;
	  if (prop == "speed:")
		 fs >> speed;
	  if (prop == "maxHealth:")
		 fs >> health;
	  if (prop == "weaponlist:"){
		 fs >> prop;
		 while (prop != "endlist"){
			weapons.push_back(prop);
			fs >> prop;
		 }
	  }
   }   
}

map<string, PlayerClass*> Player::classes;

PlayerClass* Player::getClass(string kind){
   map<string, PlayerClass*>::iterator i = classes.find(kind);
   if (i == classes.end()){
	  PlayerClass* pc = new PlayerClass(kind);
	  classes.insert(make_pair(kind,pc));
	  return pc;
   }
   return i->second;
}


Player::~Player(){
   currentWeapon = NULL;
   for (map<string, Weapon*>::iterator i = weapons.begin(); i != weapons.end(); ++i)
	  delete i->second;
}

Player::Player(string name, string playerClass) :
   Actor(TYPE_INVISIBLE, 0, 0, PLAYER_R, PLAYER_TEAM, 0, 0), weapons(), money(0), moneyChanged(false), name(name),
   respawnTimeout(0), playerClass(playerClass){
   setHealth(maxHealth = getClass(playerClass)->getHealth());
   setType(getClass(playerClass)->getImg());
   speed = getClass(playerClass)->getSpeed();
   addWeapon(getClass(playerClass)->getWeaponName(0));
   respawn();
}

bool Player::tick(){
   Actor::tick();
   if (respawnTimeout && !--respawnTimeout)
	  respawn();
	if (isUnloved() && PlayState::getInstance()->getCurrentFrame() > 2000)
		addWeapon("wXavierWeapon");
   return true;
}

void Player::giveMoney(int amount){
   money += amount;
   if (money < 0)
	  money = 0;
   moneyChanged = true;
}

int Player::getMoneyIfChanged(){
   if (moneyChanged){
	  moneyChanged = false;
	  return money;
   }
   return -1;
}

void Player::respawn(){
   if (PlayState::getInstance()->organIsAlive()){
	  setType(getClass(playerClass)->getImg());
	  int x = randBetween(MAP_WIDTH / 4, MAP_WIDTH - MAP_WIDTH / 4);
	  int y = randBetween(MAP_HEIGHT / 4, MAP_HEIGHT - MAP_HEIGHT / 4);
	  moveTo(x, y);
	  setHealth(maxHealth);
	  poisonAmount = 0;
	  slowness = 0;
   }
}

bool Player::buyWeapon(string name){
   int price = WeaponFactory::getWeaponPrice(name);
   if(money >= price) {
	  giveMoney(-price);
	  addWeapon(name);
	  return true;
   }
   return false;
}

void Player::addWeapon(string weapon){
   Weapon* w = WeaponFactory::getWeapon(weapon, PLAYER_TEAM);
   weapons.insert(make_pair(weapon, w));
   currentWeapon = w;
}

bool Player::changeWeapon(int index){ // todo: cleaner return thingy
   string weapon = getClass(playerClass)->getWeaponName(index);
   if (weapon.empty())
	  return false;
   map<string, Weapon*>::iterator i;
   i = weapons.find(weapon);
   if (i == weapons.end())
	  return buyWeapon(weapon);
   currentWeapon = i->second;
   return true;
	  
}

void Player::die(){
   setType(TYPE_PLAYER_DEAD);
   setDx(0);
   setDy(0);
   respawnTimeout = RESPAWN_TIMEOUT; // todo: increasing timeout?
   giveMoney(-DEATH_PENALTY);
}

bool Player::isLoved() const{
	if (name=="Nauktis" or name=="Nils" or name=="Balor" or name=="Neo" or name=="Oleg") {
		return true;
	}
	return false;
}

bool Player::isUnloved() const{
	if (name == "kiki" or name == "Kiki" or name == "Xavier" or name == "xavier")
		return true;
	return false;
}

void Player::shoot(double angle){
   if (isAlive())
	   if (isLoved())
		  Actor::shoot(angle, true);
	   else
		   Actor::shoot(angle);
}

void Player::move(int direction){
   if (!isAlive())
	  return;
   switch(direction){
   case DIR_RIGHT:
	  Actor::move(speed, getDy());
	  break;
   case DIR_UP:
	  Actor::move(getDx(), -speed);
	  break;
   case DIR_LEFT:
	  Actor::move(-speed, getDy());
	  break;
   case DIR_DOWN:
	  Actor::move(getDx(), speed);
	  break;
   case DIR_NOY:
	  Actor::move(getDx(), 0);
	  break;
   case DIR_NOX:
	  Actor::move(0, getDy());
	  break;
   default:
	  throw std::runtime_error("Playstate::playerMove: direction unknown");
	  break;
   }
}
