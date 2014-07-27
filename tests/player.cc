#include "../GameEngine/player.hh"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(player_money){
   Player p = Player("bob", "fighter");
   BOOST_CHECK(p.getMoney() == 0);
   p.giveMoney(200);
   BOOST_CHECK(p.getMoney() == 200);
   p.giveMoney(-500);
   BOOST_CHECK(p.getMoney() == 0);
}

BOOST_AUTO_TEST_CASE(player_buyWeapon){
   Player p("bob", "healer");
   BOOST_CHECK(!p.changeWeapon(1));
   p.giveMoney(20);
   BOOST_CHECK(p.changeWeapon(1));
   BOOST_CHECK(p.getMoney() == 10);
}

