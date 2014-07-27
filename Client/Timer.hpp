/**
 * \file Timer.hpp
 * \class Timer AbsTimer Timer.hpp
 * \author Antoine Aflalo, Oleg Bogatchev
 * \brief This class represent the timers.
 * \date 5 mars 2010
 */
#ifndef _TIMER_AND_ABS_H_
#define _TIMER_AND_ABS_H_

#include <sys/time.h> // gettimeofday(), usleep()...
#include <inttypes.h> // uint32_t on OpenSolaris

class AbsTimer
{
	protected:
	int startTicks_;
	bool started_;
	bool paused_;
	static AbsTimer* instance_;
	int maxFPS_;
    int currentFrame_;
    int pausedTicks_;
	AbsTimer();
	void start_();

	uint32_t getMilliseconds();
	void delay(uint32_t ms);
	struct timeval startedTime;
	
	public:
	static AbsTimer* getInstance();
	static void killInstance();
	
	/**
	 * Start Timer
	 */
    void start();
    /**
     * Pause timer
     */    
    void pause();
    /** 
     * Stop timer
     */
    void stop();
	/**
     * To limit the FPS (currently 60 FPS)
     */
    void limitFPS();
    /**
     * Get the tick
     */
    int getTicks();
    /**
     * Get the max Fps
     */
    int getMaxFPS();
    /**
     * Get if started
     * @return true if the timer is started else false
     */
     bool isStarted();
          /**
     * Get if paused
     * @return true if the timer is paused else false
     */
    bool isPaused();


};

class Timer : public AbsTimer
{
	private:
	Timer();
	static Timer* instance_;

	public:
	static Timer* getInstance();
	static void killInstance();
	/**
	 * Start Timer
	 */
    void start();
    /**
     * Get the tick
     */
    int getTicks();
    /**
     * Unpause timer
     */
    void unpause();


    

};



#endif
