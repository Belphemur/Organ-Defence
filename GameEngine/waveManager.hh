#ifndef _WAVEMANAGER_H_
#define _WAVEMANAGER_H_

#include <list>
using std::list;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
#include <stdexcept>


class PlayState;

class MonsterGroup{
   int number;
   string monsterName;
   double x,y;
public:
   MonsterGroup(int number, string monster, double x, double y) :
	  number(number), monsterName(monster), x(x), y(y) {}
   void trigger();
};

class Wave{
   long frame;
   list<MonsterGroup> groups;
   list<string> startingPositions;
public:
   Wave(long frame) : frame(frame) {}
   long getFrame() const {return frame;}
	void addStartingPosition(string position);
   void addGroup(MonsterGroup g) {groups.push_back(g);}
	void trigger();
};

class WaveManager{
   list<Wave> waves;
public:
   WaveManager();
   void tick(long frame);
};


#endif /* _WAVEMANAGER_H_ */
