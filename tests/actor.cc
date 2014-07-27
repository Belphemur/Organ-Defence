#include "../GameEngine/actor.hh"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(actor_hurt){
   Actor a = Actor(TYPE_PLAYER_FIGHTER, 5, 5, 5, PLAYER_TEAM, 100, 10);
   BOOST_CHECK(a.getHealth() == 100);
   a.hurt(50);
   BOOST_CHECK(a.getHealth() == 50);
   a.hurt(-25);
   BOOST_CHECK(a.getHealth() == 75);
   a.hurt(-100);
   BOOST_CHECK(a.getHealth() == 100);
}
