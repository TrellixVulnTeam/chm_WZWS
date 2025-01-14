#pragma once
#ifndef WD_CPP_
#define WD_CPP_


#include <thread>
#include "system.h"
#include "singleton.h"
#include <mutex>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace Core_Health;


class WatchDog {
	duration<int>     counter;
	thread            watchdog_thread;
	static void       WatchDogFunction();
	mutex             mtx;
	bool              is_running;

	WatchDog() : counter(CHMConfig_t::T_WATCHDOG_RESET_SEC), is_running(false){};
	friend singleton<WatchDog>;
	
public:
	
	void Start(int reset_value_in_secs){
		if (is_running == false) {
			is_running = true;
			counter = seconds(reset_value_in_secs);
			watchdog_thread = thread(&WatchDogFunction);
		}
	}
	
	void Stop(){
		if (is_running == true) {
			watchdog_thread.join();
			is_running = false;
		}
	}

	void Kick(){
		lock_guard<mutex> lock(mtx);
		counter = duration<int>(CHMConfig_t::T_WATCHDOG_RESET_SEC);
	}

	void SetResetInterval(unsigned int new_reset_interval){
		CHMConfig_t::T_WATCHDOG_RESET_SEC = new_reset_interval;
	}

	void DecrementCounter(duration<int> amount) {
		lock_guard<mutex> lock(mtx);
		counter -= (amount);
	}

	duration<int> GetCounterDurationInSecs()const{
		return counter;
	}

};

#endif