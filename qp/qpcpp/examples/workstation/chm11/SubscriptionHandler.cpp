#include "SubscriptionHandler.h"
#include <stdio.h>

bool SubscriptionHandler::IsIdInSystem(int id)const {
	if (id < 0) return false;
	//pass through the members array and check if any member has an ID = 'id'. If so, return true
	for (int i = 0; i < N_MEMBER; ++i) {
		if (members[i].id == id) return true; 
	}
	return false;
}


int SubscriptionHandler::FindFreeSpace()const {
	for (int i = 0; i < N_MEMBER; ++i) {
		if (members[i].id == UNIDENTIFIED_ID) return i;
	}
	return SYSTEM_FULL;
}

int SubscriptionHandler::GetNumberOfMembers() {
	int num_of_subscribers = 0;
	for (int i = 0; i < N_MEMBER; ++i) {
		if (members[i].id != UNIDENTIFIED_ID) {
			num_of_subscribers++;
		}
	}
	return num_of_subscribers;
}


int SubscriptionHandler::SubscribeUser(int user_id) {
	if (user_id < 0) return INVALID_INPUT;
	int index = SYSTEM_FULL;
	bool user_in_sys = IsIdInSystem(user_id);
	if (user_in_sys) printf("User %d is already in the system\n", user_id);
	else {
		// find the first free index in the members array
		index = FindFreeSpace();
		// if a free space was found, subscribe user
		if (index != SYSTEM_FULL) {
			//if we found an index in the members array not associated with another user, register the new subscriber
			members[index].id = user_id;
			//the subscription is an ALIVE signal
			members[index].keep_alive_received = true;

		}
		else printf("System full\n");
	}
	return index;
}


bool SubscriptionHandler::UnSubscribeUser(int index) {
	bool status = false;
	if((index < 0) || (index >= N_MEMBER)) return false;
	//update subscribers array to show that a user has unsubscribed
	if (members[index].id != UNIDENTIFIED_ID) {
		members[index].id = UNIDENTIFIED_ID;
		status = true;
		//check if the user has sent an ALIVE_SIG signal recently; if set 'keep_alive_received' to false
		if (members[index].keep_alive_received == true) {
			members[index].keep_alive_received = false;
		}
	}
	return status;
}



void SubscriptionHandler::LogUnResponsiveUsersAndReset() {
	bool subscriber_is_unresponsive = false;
	//for each user set the keep_alive_received to false for next cycle 
	for (int i = 0; i < N_MEMBER; ++i) {
		// A subscriber is unresponsive if he's registered and hasn't sent an ALIVE signal
		subscriber_is_unresponsive = ((members[i].id != UNIDENTIFIED_ID) && (members[i].keep_alive_received == false));
		if (subscriber_is_unresponsive) printf("Watchdog wasn't kicked because member id %d didn't send ALIVE signal\n", members[i].id);
		// reset the 'keep_alive_received' parameter for all users 
		members[i].keep_alive_received = false;
	}
}

bool SubscriptionHandler::AreAllMembersResponsive() {
	bool subscriber_is_unresponsive = false;
	// For each member check if there has been an ALIVE signal in the current cycle
	for (int i = 0; i < N_MEMBER; ++i) {
		subscriber_is_unresponsive = ((members[i].id != UNIDENTIFIED_ID) && (members[i].keep_alive_received == false));
		if (subscriber_is_unresponsive) return false;
	}
	return true;
}

void SubscriptionHandler::UpdateAliveStatus(int sys_id) {
	if ((sys_id < 0) || (sys_id >= N_MEMBER)) return;
	//check if the user hasn't sent an ALIVE_SIG already: if not, update the members array in the appropriate index
	if(members[sys_id].keep_alive_received == false) {
		members[sys_id].keep_alive_received = true;
	}
}