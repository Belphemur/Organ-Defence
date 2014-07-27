#include "organ.hh"
#include "playstate.hh"

void Organ::die(){
   PlayState::getInstance()->gameover();
   setType(TYPE_DEAD_ORGAN);
}

bool Organ::tick(){
   if (getImg() == TYPE_ORGAN_LIVER)
	  if (!(rand()%100))
		 hurt(-1);
   return Actor::tick();
}
