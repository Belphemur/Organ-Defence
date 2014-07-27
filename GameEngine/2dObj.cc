#include "2dObj.hh"
#include "playstate.hh"

unsigned int TwoDObject::currentUid = 0;

TwoDObject::~TwoDObject(){
  if (sent && isVisible())
     PlayState::getInstance()->addDeletion(uid);
}
