#include "player.hh"
#include "consts.hh"
#include "brain.hh"
#include "weapon.hh"


class AIPlayer : public Player{
   Brain* brain;
public:
   AIPlayer(string c) : Player("HAL", c), brain(NULL){
	  brain = new FakePlayerBrain(this);
	  addWeapon("wBasic");
   }
   bool tick(){
	  brain->doSomething();
	  changeWeapon(1);
	  return Player::tick();
   }
};
