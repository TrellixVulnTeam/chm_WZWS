#include "watchdog.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include "system.h"
void WatchDog::WatchDogFunction() {

	WatchDog& wd = WatchDog::getInstance();
	int secs_in_dur = 0, last_wd = 0,curr_wd = 0;
	std::chrono::duration<int> dur;
	std::chrono::system_clock::time_point now;

	//find the start time of the thread and initialize the variable 'last_time'
	std::chrono::system_clock::time_point last_time = std::chrono::system_clock::now();

	while (1) {
		//find the current time : now
		now = std::chrono::system_clock::now();
        //find the difference in time (ie duration) between now and last time in seconds time unit
		dur = std::chrono::duration_cast<std::chrono::seconds>(now - last_time);
		

		//decrease the watchdog by the number of seconds calculated
		wd.Decrement(dur.count());
		//advance the 'last_time' variable by the number of seconds calculated
		last_time += dur;

		//make the thread go to sleep  for 500 msecs
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		/*used for debugging:*****************************************************************************************************************! TO BE REMOVED!*/
		curr_wd = wd.GetCounter();
		if (curr_wd != last_wd) {
			std::cout << "wd = " << wd.GetCounter() << std::endl;
			last_wd = curr_wd;
		}
		/************************************************************************/
	}
}