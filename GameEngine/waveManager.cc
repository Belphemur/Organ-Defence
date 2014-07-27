#include "waveManager.hh"
#include "monster.hh"
#include "game.hh"

#include <cstdlib>
#include <algorithm>

void Wave::trigger(){
//	TODO clean comments
	/*std::cout << "AA : New wave coming from ";
	//string todo_removeMe;
	for (list<string>::iterator it=startingPositions.begin(); it!=startingPositions.end(); it++) {
		todo_removeMe += *it;
		std::cout << " & " << *it;
	}
	std::cout << std::endl;*/
	Game::getInstance()->warnPlayersOnNewWave(startingPositions);
	for (list<MonsterGroup>::iterator i = groups.begin();
		 i != groups.end(); ++i)
		(*i).trigger();
	
}

void Wave::addStartingPosition(string position){
	//std::cout << "parsing position " << position << std::endl;
	list<string>::iterator it;
	it = std::find(startingPositions.begin(), startingPositions.end(), position);
	/*		  bool found = false
	 for ( it=startingPositions.begin() ; it != startingPositions.end() && !found; it++ )
	 found = (*it == place);
	 if (!found) {
	 startingPositions.push_back(place)
	 }*/
	if (it == startingPositions.end()) {
		startingPositions.push_back(position);
	}
	
}

void WaveManager::tick(long frame){
   for(list<Wave>::iterator i = waves.begin(); i != waves.end(); ++i){
	  if ((*i).getFrame() == frame){
		 (*i).trigger();
		 waves.erase(i);
		 break;
	  }
	  if ((*i).getFrame() > frame)
		 break;
   }
}

WaveManager::WaveManager() {
   string fn = DATA_DIR + "waves";
   ifstream fs(fn.c_str());
   if (!fs)
	  throw std::runtime_error("couldn't load wave descriptor:" + fn);
   long frameNum;
   while(fs >> frameNum){
	  Wave w(frameNum);
	  while (true){
		 string nextMonster;
		 fs >> nextMonster;
		 if (nextMonster == "end_wave")
			break;
		 int number;
		 fs >> number;
		 string place;
		 fs >> place;
		  double x,y;
		  w.addStartingPosition(place);
		  
		  if (place == "NE"){	// todo: dump all this in a map or something
			  x = MAP_WIDTH + 2 * MONSTER_DISP;
			  y = -2 * MONSTER_DISP;
		  }
		  else if (place == "NW"){
			  y = -2 * MONSTER_DISP;
			  x = -2 * MONSTER_DISP;
		  }
		  else if(place == "SE"){
			  x = MAP_WIDTH + 2 * MONSTER_DISP;
			  y = MAP_HEIGHT + 2 * MONSTER_DISP;
		  }
		  else if (place == "SW"){
			  x = -2 * MONSTER_DISP;
			  y = MAP_HEIGHT + 2 * MONSTER_DISP;
		  }
		  else
			  throw std::runtime_error("unknown place: " + place);
		 MonsterGroup g(number, nextMonster, x, y);
		 w.addGroup(g);
	  }
	  waves.push_back(w);
   }
}

void MonsterGroup::trigger(){
   for (int i = 0; i < number; ++i) {
	  x += (rand() % (2 * MONSTER_DISP)) - MONSTER_DISP;
	  y += (rand() % (2 * MONSTER_DISP)) - MONSTER_DISP;
	  MonsterFactory::create(monsterName, x, y);
   }
}
