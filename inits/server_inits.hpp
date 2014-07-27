#ifndef __S_INITS_HPP_
#define __S_INITS_HPP_

#include <string>

int nbClientsMax(int argc, char** argv);

void initServerNetworkManager();
void initAbsTimer();
void initProcessManager();
void initGameEngine();
void endOfServer(int);
#endif
