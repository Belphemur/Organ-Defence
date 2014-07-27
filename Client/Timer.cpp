#ifdef CLIENT_SDL
#include "SDL/SDL.h"
#endif
#include "Timer.hpp"
#include <stdexcept>

Timer* Timer::instance_ = NULL;
AbsTimer* AbsTimer::instance_ = NULL;

AbsTimer::AbsTimer()
{
    startTicks_ = 0;
    started_ = false;
    maxFPS_ = 60;
    currentFrame_ = 0;
    paused_ = false;
	pausedTicks_ = 0;
}

AbsTimer* AbsTimer::getInstance() 
{
	if (not instance_) instance_ = new AbsTimer(); 
	return instance_; 
}
void AbsTimer::start_()
{
		started_ = true;
		#ifdef CLIENT_SDL
			startTicks_ = SDL_GetTicks();
		#else
			gettimeofday(&startedTime, NULL);
			startTicks_ = getMilliseconds();
		#endif
}

void AbsTimer::start()
{
	if(! isStarted())
	{
		this->start_();
	}
	else
		throw std::runtime_error("Absolute Timer already started : can't be started again");
}

void AbsTimer::pause()
{
    if( ( started_ == true ) && ( paused_ == false ) )
    {
        paused_ = true;
        #ifdef CLIENT_SDL
			pausedTicks_ = SDL_GetTicks() - startTicks_;
        #else
			pausedTicks_ = getMilliseconds() - startTicks_;
        #endif
	}
}

void AbsTimer::stop()
{
    started_ = false;

    paused_ = false;
}
bool AbsTimer::isPaused()
{
    return paused_;
}
uint32_t AbsTimer::getMilliseconds()
{
	/*uint32_t ticks;
	struct timeval t;
	gettimeofday(&t, NULL);
	ticks=(t.tv_sec)*1000+(t.tv_usec)/1000;
	return ticks;*/
	uint32_t ticks;
	struct timeval now;
	gettimeofday(&now, NULL);
	ticks=(now.tv_sec-startedTime.tv_sec)*1000+(now.tv_usec-startedTime.tv_usec)/1000;
	return(ticks);
}

void AbsTimer::delay(uint32_t ms)
{
	uint32_t then, now, elapsed;
	
	then = getMilliseconds();

	do {
		now = getMilliseconds();
		elapsed = (now-then);
		then = now;
		if ( elapsed >= ms ) {
			break;
		}
		ms -= elapsed;

	} while ( true );
}

int AbsTimer::getTicks()
{
    if( started_ == true )
    {
        if( paused_ == true )
        {
            return pausedTicks_;
        }
        else
        {
            #ifdef CLIENT_SDL
				return SDL_GetTicks() - startTicks_;
			#else
				return getMilliseconds() - startTicks_;
			#endif
        }
    }
    return 0;
}

void AbsTimer::limitFPS()
{
	if(getTicks() < 1000 / maxFPS_)
	#ifdef CLIENT_SDL
		SDL_Delay( (1000 / maxFPS_) - getTicks());
	#else
		delay( (1000 / maxFPS_) - getTicks());
	#endif
}

int AbsTimer::getMaxFPS()
{
    return maxFPS_;
}

bool AbsTimer::isStarted()
{
    return started_;
}


void AbsTimer::killInstance()
{
	delete instance_;
}

/* Timer */
Timer::Timer()
{
}

Timer* Timer::getInstance() 
{
	if (not instance_) instance_ = new Timer(); 
	return instance_; 
}

void Timer::killInstance()
{
	delete instance_;
}

void Timer::start()
{
	AbsTimer::start_();
    paused_ = false;
}

void Timer::unpause()
{
    if( paused_ == true )
    {
        paused_ = false;
        #ifdef CLIENT_SDL
			startTicks_ = SDL_GetTicks() - pausedTicks_;
        #else
			startTicks_ = getMilliseconds() - pausedTicks_;
        #endif
        pausedTicks_ = 0;
    }
}




int Timer::getTicks()
{
    if( started_ == true )
    {
        if( paused_ == true )
        {
            return pausedTicks_;
        }
        else
        {
            #ifdef CLIENT_SDL
				return SDL_GetTicks() - startTicks_;
			#else
				return getMilliseconds() - startTicks_;
			#endif
        }
    }
    return 0;
}
