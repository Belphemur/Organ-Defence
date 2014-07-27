#include "aiActor.hh"
#include "brain.hh"


bool AiActor::tick(){
   Actor::tick();
   if (brain)
	  brain->doSomething();
   if (health <= 0)
	  return false;
   return true;
}


AiActor::~AiActor(){
   delete brain;
}
