#include "../GameEngine/2dObj.hh"
#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(collide){
   TwoDObject o1(5, 5, 5);
   TwoDObject o2(0, 5, 5);
   TwoDObject o3(25, 5, 5);
   BOOST_CHECK(o1.collide(o2));
   BOOST_CHECK(!o1.collide(o3));
}

BOOST_AUTO_TEST_CASE(inGameArea){
   TwoDObject o1(20, 20, 5);
   TwoDObject o2(-10, 5, 5);
   TwoDObject o3(5, 5, 10);
   BOOST_CHECK(o1.inGameArea());
   BOOST_CHECK(!o2.inGameArea());
   BOOST_CHECK(!o3.inGameArea());
}

BOOST_AUTO_TEST_CASE(move){
   TwoDObject o1(20, 20, 5);
   TwoDObject o2(20, 20, 5, -5, 0);
   o1.move();
   o2.move();
   BOOST_CHECK(o1.getX() == 20 && o1.getY() == 20);
   BOOST_CHECK(o2.getX() == 15 && o2.getY() == 20);
}

BOOST_AUTO_TEST_CASE(moveTo){
   TwoDObject o1(20, 20, 5);
   BOOST_CHECK(o1.getX() == 20 && o1.getY() == 20);
   o1.moveTo(55, 55);
   BOOST_CHECK(o1.getX() == 55 && o1.getY() == 55);
}

BOOST_AUTO_TEST_CASE(isVisible){
   TwoDObject o1(20, 20, 5);
   TwoDObject o2(20, 20, 5, 0, 0, TYPE_MINE);
   BOOST_CHECK(!o1.isVisible());
   BOOST_CHECK(o2.isVisible());
}

BOOST_AUTO_TEST_CASE(getImg){
   TwoDObject o1(20, 20, 5);
   TwoDObject o2(20, 20, 5, 0, 0, TYPE_MINE);
   BOOST_CHECK(o1.getImg() == TYPE_INVISIBLE);
   BOOST_CHECK(o2.getImg() == TYPE_MINE);
}

BOOST_AUTO_TEST_CASE(dist){
   TwoDObject o1(0, 0, 5);
   TwoDObject o2(0, 5, 3);
   TwoDObject o3(10, 10, 56);
   BOOST_CHECK_EQUAL(o1.dist(&o2), 5);
   BOOST_CHECK(abs(o2.dist(&o3) - 11.18) < 0.01);
}

BOOST_AUTO_TEST_CASE(setDx){
   TwoDObject o1(0, 0, 5);
   BOOST_CHECK(o1.hasBeenModified());
   o1.getDrawable();
   BOOST_CHECK(!o1.hasBeenModified());
   o1.setDx(5);
   BOOST_CHECK(o1.hasBeenModified());
   o1.move();
   BOOST_CHECK(o1.getX() == 5);
}
