#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
using std::string;
#include <map>
using std::map;
#include <vector>
using std::vector;

#include "consts.hh"
#include "actor.hh"
#include "../Network/Message.hpp" // Needed for direction enum in move


class PlayState;

class PlayerClass{
   vector<string> weapons;
   unsigned health;
   double speed;
   int img;
   void load(string kind);
public:
   PlayerClass(string kind) {load(kind);}
   unsigned getHealth() const {return health;}
   double getSpeed() const {return speed;}
   Drawable_Type getImg() const {return static_cast<Drawable_Type>(img);}
   string getWeaponName(unsigned index) const {if (index < weapons.size()) return weapons[index]; return "";}
};


class Player : public Actor{
   static map<string, PlayerClass*> classes;
   static PlayerClass* getClass(string name);
   map<string, Weapon*> weapons;
   int money;
   bool moneyChanged;
   string name;
   int respawnTimeout;
   string playerClass;
protected:
   void respawn();
   bool buyWeapon(string name);
public:
   Player(string name, string playerClass);
   bool isLoved() const;
   bool isUnloved() const;
   void addWeapon(string weaponName);
   bool changeWeapon(int index);
   void giveMoney(int amount);
   bool tick();
   void die();
   void shoot(double angle);
   void move(int direction);
   int getMoneyIfChanged();
   int getMoney() const {return money;}
   virtual ~Player();
};

#endif /* _PLAYER_H_ */
