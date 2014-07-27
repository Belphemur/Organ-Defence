#include <cstdlib>
#include <algorithm>
#include "util.hh"
using std::swap;

int randBetween(int low, int high){
   if (low > high)
	  swap(low, high);
   return rand() % (high - low + 1) + low;
}
